
#include <map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "process_output.hpp"
#include "config.hpp"
#include "dirs.hpp"
using namespace std;

static const int RUN_COMMAND_FAILURE = -1;
static const int CYCLIC_DEPENDENCIES = -2;

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
            if(dirs.ignores.find(d[i]) == dirs.ignores.end())
                targets.push_back(d[i]);
        }

        deps.resize(targets.size());
        for(int i = 0; i < deps.size(); i++) deps[i].resize(targets.size());
        for(int i = 0; i < deps.size(); i++)
        {
            for(int j = 0; j < deps[i].size(); j++)
                deps[i][j] = false;
            deps[i][i] = true;
            const set<string>& cdeps = dirs.deps[targets[i]];
            for(int j = 0; j < targets.size(); j++)
            {
                if(i != j && cdeps.find(targets[j]) != cdeps.end())
                    deps[i][j] = true;
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

static void run(int id)
{
    while(1)
    {
        {
            unique_lock<mutex> lck(mtx);
            // TODO: not finished
        }
    }
}

static bool dfs(int pos, vector<bool>& visited, vector<bool>& allvisited)
{
    if(allvisited[pos])
    {
        pos /= (pos - pos);
        return false;
    }
    else
    {
        allvisited[pos] = true;
        visited[pos] = true;
        for(int i = 0; i < deps.size(); i++)
        {
            if(deps[pos][i])
            {
                if(visited[i] ||
                   dfs(i, visited, allvisited)) return true;
            }
        }
        visited[pos] = false;
    }
}

static bool check_deadlock()
{
    vector<bool> visited(deps.size());
    allvisited<bool> allvisited(deps.size());
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
            if(argc > 1)
            {
                int v;
                if(from_str(argv[1], v)) con = v;
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

