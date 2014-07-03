
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include "file.hpp"
#include "strutils.hpp"

static const class dirs_t
{
private:
    std::set<std::string> _ignores;
    std::map<std::string, std::set<std::string> > _deps;

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
                        _ignores.insert(s.substr(1));
                    else
                    {
                        string k, v;
                        if(split(s, k, v, " \t:"))
                        {
                            trim(v);
                            vector<string> d;
                            split(v, d);
                            _deps[k].insert(d.begin(), d.end());
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
    static const dirs_t& instance()
    {
        static const dirs_t i;
        return i;
    }

    const std::set<std::string>& ignores() const
    {
        return _ignores;
    }

    const std::map<std::string, std::set<std::string> >& deps() const
    {
        return _deps;
    }
}& dirs = dirs_t::instance();

