
#pragma once
#if 0
#include <boost/filesystem/path.hpp>
#endif
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "file.hpp"
#if 0
using namespace boost::filesystem;
#endif

static const class config_t
{
private:
    void read_file(const std::string& file)
    {
        using namespace std;
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
                    else if(k == "cc_c")
                        _cc_c = v;
                    else if(k == "cc")
                        _cc = v;
                    else if(k == "cc_flag")
                        _cc_flag = v;
                    else if(k == "dlink")
                        _dlink = v;
                    else if(k == "list")
                        _list = v;
                    else if(k == "main")
                        _main = v;
                    else if(k == "objs")
                        _objs = v;
                    else if(k == "out")
                        _out = v;
                    else if(k == "dlibs")
                        _dlibs = v;
                    else if(k == "rm")
                        _rm = v;
                }
            }
        }

        if(_cc_c.empty() && !_cc.empty())
            _cc_c = _cc + " -c";
    }

    std::string _cc_m;
    std::string _cc_c;
    std::string _cc;
    std::string _cc_flag;
    std::string _dlink;
    std::string _list;
    std::string _main;
    std::string _objs;
    std::string _out;
    std::string _dlibs;
    std::string _rm;

    const static std::string default_cc_m;
    const static std::string default_cc_c;
    const static std::string default_cc;
    const static std::string default_cc_flag;
    const static std::string default_dlink;
    const static std::string default_list;
    const static std::string default_main;
    const static std::string default_objs;
    const static std::string default_out;
    const static std::string default_dlibs;
    const static std::string default_rm;

public:
#define return_value(x) \
    const std::string& x() const { \
        return (_##x.empty() ? default_##x : _##x); }
    return_value(cc_m);
    return_value(cc_c);
    return_value(cc);
    return_value(cc_flag);
    return_value(dlink);
    return_value(list);
    return_value(main);
    return_value(objs);
    return_value(out);
    return_value(dlibs);
    return_value(rm);
#undef return_value

    static const config_t instance;
private:
    config_t()
    {
        using namespace std;
        const char* autocompile = ".autocompile";
#if 0
        path file_path(string(".") + autocompile);
#endif
        if(getenv("HOMEPATH") != nullptr)
        {
            read_file(string(getenv("HOMEPATH")) + "/" + autocompile);
#if 0
            path p(getenv("HOMEPATH"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        if(getenv("HOME") != nullptr)
        {
            read_file(string(getenv("HOME")) + "/" + autocompile);
#if 0
            path p(getenv("HOME"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        read_file(autocompile);
    }
} &config = config_t::instance;

