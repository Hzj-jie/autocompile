
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include "file.hpp"
#include "strutils.hpp"

static const dirs_t
{
private:
    std::set<std::string> ignores;
    std::map<std::string, std::set<std::string> > deps;

    void read_file(const std::string& file)
    {
        using namespace std;
        vector<string> lines;
        if(read_all_lines(file, lines))
        {
            for(int i = 0; i < lines.size(); i++)
            {
                const string& s = lines[i];
                if(!s.empty() && s[0] != '#')
                {
                    if(s[0] == '-')
                        ignores.insert(s.substr(1));
                    else
                    {
                        string k, v;
                        if(split(s, k, v, " \t:"))
                        {
                            trim(v);
                            vector<string> d;
                            split(v, d);
                            deps[k].insert(deps[k].end(), d.begin(), d.end());
                        }
                        else cerr << "unknown command " << s << " in file " << file << ", ignore" << endl;
                    }
                }
            }
        }
    }

    dirs_t()
    {
        read_file("dirs");
    }

public:
    static const& dirs_t instance()
    {
        static const dirs_t i;
        return i;
    }
}& dirs = dirs_t::instance();

