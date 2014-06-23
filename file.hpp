
#pragma once
#include <fstream>
#include <string>
using namespace std;

namespace
{
    bool read_all_lines(ifstream& is, vector<string>& o)
    {
        if(is)
        {
            string i;
            while(getline(is, i)) o.push_back(i);
            return true;
        }
        else return false;
    }
}

bool read_all_lines(const string& file, vector<string>& o)
{
    ifstream is(file, ifstream::in);
    bool r = read_all_lines(is, o);
    is.close();
    return r;
}

