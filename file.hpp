
#pragma once
#include <fstream>
#include <string>

namespace
{
    using namespace std;
    static bool read_all_lines(ifstream& is, vector<string>& o)
    {
        if(is)
        {
            o.clear();
            string i;
            while(getline(is, i)) o.push_back(i);
            return true;
        }
        else return !is.bad();
    }
}

static bool read_all_lines(const std::string& file, std::vector<std::string>& o)
{
    using namespace std;
    ifstream is(file, ifstream::in);
    bool r = read_all_lines(is, o);
    is.close();
    return r;
}

static string replace_suffix(const std::string& s, const std::string& new_suffix)
{
    using namespace std;
    int i = s.find_last_of('.');
    if(i == string::npos)
        return s;
    else
        return s.substr(0, i + 1) + new_suffix;
}

