
#pragma once
#include <fstream>
#include <string>
using namespace std;

namespace
{
    static bool read_all_lines(ifstream& is, vector<string>& o)
    {
        if(is)
        {
            o.clear();
            string i;
            while(getline(is, i)) o.push_back(i);
            return true;
        }
        else return false;
    }
}

static bool read_all_lines(const string& file, vector<string>& o)
{
    ifstream is(file, ifstream::in);
    bool r = read_all_lines(is, o);
    is.close();
    return r;
}

static string replace_suffix(const string& s, const string& new_suffix)
{
    int i = s.find_last_of('.');
    if(i == string::npos)
        return s;
    else
        return s.substr(0, i + 1) + new_suffix;
}

