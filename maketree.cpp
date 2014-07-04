
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <boost/format.hpp>
#include <mutex>
#include "process_output.hpp"
#include "config.hpp"
#include "dirs.hpp"
using namespace std;
using namespace boost;

static const int RUN_COMMAND_FAILURE = -1;
static const int CYCLIC_DEPENDENCIES = -2;

static bool verify = false;
static bool clean = false;
static vector<string> targets;
static vector<vector<bool> > deps;
static mutex mtx;

static bool init_targets()
{
    targets.clear();
    vector<string> d;
    if(process_output(config.list_dir(), d))
    {
        for(int i = 0; i < d.size(); i++)
        {
            if(dirs.ignores().find(d[i]) == dirs.ignores().end())
                targets.push_back(d[i]);
        }

        deps.resize(targets.size());
        for(int i = 0; i < deps.size(); i++) deps[i].resize(targets.size());
        for(int i = 0; i < deps.size(); i++)
        {
            for(int j = 0; j < deps[i].size(); j++)
                deps[i][j] = false;
            deps[i][i] = true;
            auto it = dirs.deps().find(targets[i]);
            if(it != dirs.deps().end())
            {
                const set<string>& cdeps = (*it).second;
                for(int j = 0; j < targets.size(); j++)
                {
                    if(i != j && cdeps.find(targets[j]) != cdeps.end())
                        deps[i][j] = true;
                }
            }
        }

        return true;
    }
    else return false;
}

static void mark_deps(int i)
{
    for(int j = 0; j < deps.size(); j++)
        deps[j][i] = false;
}

static string merge_command(const string& path, const string& cmd)
{
    return (format(config.command_surround()) %
                   (format(config.command_separator()) %
                           (format(config.cd()) % path).str() %
                           cmd).str()).str();
}

static bool process_output(const string& path,
                           const string& cmd,
                           vector<string>& o)
{
    return process_output(merge_command(path, cmd), o);
}

static bool has_makefile(const string& p)
{
    vector<string> o;
    return process_output(p, config.list_makefile(), o) &&
           !o.empty();
}

static void run_cmd(int id, const string& p, const string& cmd, const string& cmd_clean)
{
    string c = merge_command(p, clean ? cmd_clean : cmd);
    cout << id << ": starts command " << c << endl;
    if(system_available && !verify)
        system(c.c_str());
}

static void run_make(int id, const string& p)
{
    run_cmd(id, p, config.make(), config.make_clean());
}

static void run_maketree(int id, const string& p)
{
    run_cmd(id, p, config.maketree(), config.maketree_clean());
}

static void run(int id)
{
    while(1)
    {
        int selected = -1;
        {
            unique_lock<mutex> lck(mtx);
            for(int i = 0; i < deps.size(); i++)
            {
                if(deps[i][i])
                {
                    int j;
                    for(j = 0; j < deps[i].size(); j++)
                    {
                        if(i != j && deps[i][j])
                            break;
                    }
                    if(j == deps[i].size())
                    {
                        selected = i;
                        deps[i][i] = false;
                        break;
                    }
                }
            }
        }
        if(selected >= 0)
        {
            cout << id << ": starts target " << targets[selected] << endl;
            if(has_makefile(targets[selected]))
                run_make(id, targets[selected]);
            else run_maketree(id, targets[selected]);
            mark_deps(selected);
        }
        else
        {
            cout << id << ": no more targets to run, finished" << endl;
            break;
        }
    }
}

static bool dfs(int pos, vector<bool>& visited, vector<bool>& allvisited)
{
    if(allvisited[pos])
    {
        pos -= pos;
        pos /= pos;
        return false;
    }
    else
    {
        allvisited[pos] = true;
        visited[pos] = true;
        for(int i = 0; i < deps.size(); i++)
        {
            if(i != pos && !allvisited[i] && deps[pos][i])
            {
                if(visited[i] ||
                   dfs(i, visited, allvisited)) return true;
            }
        }
        visited[pos] = false;
        return false;
    }
}

static bool check_deadlock()
{
    vector<bool> visited(deps.size());
    vector<bool> allvisited(deps.size());
    for(int i = 0; i < deps.size(); i++)
    {
        if(!allvisited[i] && dfs(i, visited, allvisited))
            return true;
    }
    return false;
}

int main(int argc, const char* const* const argv)
{
    if(init_targets())
    {
        if(check_deadlock())
        {
            cerr << "cyclic dependencies detected" << endl;
            return CYCLIC_DEPENDENCIES;
        }
        else
        {
            int con = thread::hardware_concurrency();
            for(int i = 1; i < argc; i++)
            {
                if(string("-v") == argv[i])
                    verify = true;
                else if(string("-c") == argv[i])
                    clean = true;
                else
                {
                    int v;
                    if(from_str(argv[1], v)) con = v;
                }
            }
            vector<thread> threads;
            for(int i = 0; i < con - 1; i++)
                threads.push_back(thread(run, i));
            run(con - 1);
            for(int i = 0; i < con - 1; i++)
                threads[i].join();
        }
    }
    else return RUN_COMMAND_FAILURE;
}

