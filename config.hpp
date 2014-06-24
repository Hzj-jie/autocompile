
#pragma once
#if 0
#include <boost/filesystem/path.hpp>
#endif
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "file.hpp"
using namespace std;
#if 0
using namespace boost::filesystem;
#endif

class config_t
{
private:
    void read_file(const string& file)
    {
        vector<string> lines;
        if(read_all_lines(file, lines))
        {
            for(int i = 0; i < lines.size(); i++)
            {
                const string& s = lines[i];
                if(!s.empty())
                {
                    size_t p = s.find_first_of(" \t");
                    string k, v;
                    if(p == string::npos)
                    {
                        k = s;
                    }
                    else
                    {
                        k = s.substr(0, p);
                        v = s.substr(p + 1);
                    }
                    if(k == "cc_m")
                        _cc_m = v;
                    else if(k == "cc")
                        _cc = v;
                    else if(k == "list")
                        _list = v;
                    else if(k == "main")
                        _main = v;
                    else if(k == "objs")
                        _objs = v;
                }
            }
        }
    }

    string _cc_m;
    string _cc;
    string _list;
    string _main;
    string _objs;

    const static string default_cc_m;
    const static string default_cc;
    const static string default_list;
    const static string default_main;
    const static string default_objs;

public:
#define return_value(x) \
    const string& x() const { \
        return (_##x.empty() ? default_##x : _##x); }
    return_value(cc_m);
    return_value(cc);
    return_value(list);
    return_value(main);
    return_value(objs);
#undef return_value

    config_t()
    {
        const char* autocompile = "autocompile";
#if 0
        path file_path(string(".") + autocompile);
#endif
        if(getenv("HOMEPATH") != nullptr)
        {
            read_file(string(getenv("HOMEPATH")) + "/" + "." + autocompile);
#if 0
            path p(getenv("HOMEPATH"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        if(getenv("HOME") != nullptr)
        {
            read_file(string(getenv("HOME")) + "/" + "." + autocompile);
#if 0
            path p(getenv("HOME"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        read_file(autocompile);
    }
} config;

const string config_t::default_cc_m = "g++ -M";
const string config_t::default_cc = "g++";
const string config_t::default_list = "ls -1 *.cpp *.c";
const string config_t::default_main = "main.cpp";
const string config_t::default_objs = "";

