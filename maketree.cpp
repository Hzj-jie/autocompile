
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <boost/format.hpp>
#include <mutex>
#include <chrono>
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
static mutex omtx;

static bool init_targets()
{
    targets.clear();
    vector<string> d;
    if(process_output(config.list_dir(), d))
    {
        for(size_t i = 0; i < d.size(); i++)
        {
            if(dirs.ignores().find(d[i]) == dirs.ignores().end())
                targets.push_back(d[i]);
        }

        deps.resize(targets.size());
        for(size_t i = 0; i < deps.size(); i++) deps[i].resize(targets.size());
        for(size_t i = 0; i < deps.size(); i++)
        {
            for(size_t j = 0; j < deps[i].size(); j++)
                deps[i][j] = false;
            deps[i][i] = true;
            auto it = dirs.deps().find(targets[i]);
            if(it != dirs.deps().end())
            {
                const set<string>& cdeps = (*it).second;
                for(size_t j = 0; j < targets.size(); j++)
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
    for(size_t j = 0; j < deps.size(); j++)
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
    if(system_available && !verify)
    {
        int r = system(c.c_str());
        unique_lock<mutex> lck(omtx);
        cout << id << ": command " << c << " returns " << r << endl;
    }
    else
    {
        unique_lock<mutex> lck(omtx);
        cout << id << ": starts command " << c << endl;
    }
}

static void run_make(int id, const string& p)
{
    run_cmd(id, p, config.make(), config.make_clean());
}

static void run_maketree(int id, const string& p)
{
    run_cmd(id, p, config.maketree(), config.maketree_clean());
}

void select_target(int& selected, int& unfinished)
{
    selected = -1;
    unfinished = 0;
    unique_lock<mutex> lck(mtx);
    for(size_t i = 0; i < deps.size(); i++)
    {
        if(deps[i][i])
        {
            unfinished++;
            size_t j;
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

static void run(int id)
{
    while(1)
    {
        int selected = -1;
        int unfinished = 0;
        select_target(selected, unfinished);
        if(selected >= 0)
        {
            {
                unique_lock<mutex> lck(omtx);
                cout << id << ": starts target " << targets[selected] << endl;
            }
            if(has_makefile(targets[selected]))
                run_make(id, targets[selected]);
            else run_maketree(id, targets[selected]);
            mark_deps(selected);
        }
        else if(unfinished <= id)
        {
            {
                unique_lock<mutex> lck(omtx);
                cout << id << ": no more targets to run, finished" << endl;
            }
            break;
        }
        else
        {
            {
                unique_lock<mutex> lck(omtx);
                cout << id << ": pending dependencies to finish" << endl;
            }
            using namespace std::chrono;
            while(1)
            {
                select_target(selected, unfinished);
                if(selected >= 0 || unfinished <= id) break;
                else this_thread::sleep_for(seconds(1));
            }
        }
    }
}

static bool dfs(size_t pos, vector<bool>& visited, vector<bool>& allvisited)
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
        for(size_t i = 0; i < deps.size(); i++)
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
    for(size_t i = 0; i < deps.size(); i++)
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
            {
                unique_lock<mutex> lck(omtx);
                cerr << "cyclic dependencies detected" << endl;
            }
            return CYCLIC_DEPENDENCIES;
        }
        else
        {
            size_t con = min<size_t>(thread::hardware_concurrency(), targets.size());
            for(int i = 1; i < argc; i++)
            {
                if(string("-v") == argv[i])
                    verify = true;
                else if(string("-c") == argv[i])
                    clean = true;
                else
                {
                    size_t v;
                    if(from_str(argv[1], v)) con = v;
                }
            }
            vector<thread> threads;
            for(size_t i = 0; i < con - 1; i++)
                threads.push_back(thread(run, i));
            run(con - 1);
            for(size_t i = 0; i < con - 1; i++)
                threads[i].join();
        }
    }
    else return RUN_COMMAND_FAILURE;
}

