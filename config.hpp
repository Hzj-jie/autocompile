
#pragma once
#if 0
#include <boost/filesystem/path.hpp>
#endif
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "file.hpp"
#include "strutils.hpp"
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
            for(size_t i = 0; i < lines.size(); i++)
            {
                const string& s = lines[i];
                if(!s.empty())
                {
                    string k, v;
                    split(s, k, v);
                    if(k == "cc_m" ||
                       k == "cc-m" ||
                       k == "compiler-dependency" ||
                       k == "compiler-dependencies" ||
                       k == "cc_M" ||
                       k == "cc-M")
                        _cc_m = v;
                    else if(k == "cc_c" ||
                            k == "cc-c" ||
                            k == "compile")
                        _cc_c = v;
                    else if(k == "cc" ||
                            k == "compile-link")
                        _cc = v;
                    else if(k == "cc_flag" ||
                            k == "cc-flag" ||
                            k == "compiler-flag" ||
                            k == "compiler-option")
                        _cc_flag = v;
                    else if(k == "dlink" ||
                            k == "dynamic-link")
                        _dlink = v;
                    else if(k == "list")
                        _list = v;
                    else if(k == "main")
                        _main = v;
                    else if(k == "objs" ||
                            k == "libs" ||
                            k == "static-libs")
                        _objs = v;
                    else if(k == "out" ||
                            k == "output")
                        _out = v;
                    else if(k == "dlibs" ||
                            k == "dynamic-libs")
                        _dlibs = v;
                    else if(k == "rm" ||
                            k == "remove")
                        _rm = v;
                    else if(k == "list_dir" ||
                            k == "list-dir")
                        _list_dir = v;
                    else if(k == "list_makefile" ||
                            k == "list-makefile")
                        _list_makefile = v;
                    else if(k == "cd")
                        _cd = v;
                    else if(k == "command_separator" ||
                            k == "command-separator")
                        _command_separator = v;
                    else if(k == "make")
                        _make = v;
                    else if(k == "maketree")
                        _maketree = v;
                    else if(k == "make_clean" ||
                            k == "make-clean")
                        _make_clean = v;
                    else if(k == "maketree_clean" ||
                            k == "maketree-clean")
                        _maketree_clean = v;
                    else if(k == "command_surround" ||
                            k == "command-surround")
                        _command_surround = v;
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
    std::string _list_dir;
    std::string _list_makefile;
    std::string _cd;
    std::string _command_separator;
    std::string _make;
    std::string _maketree;
    std::string _make_clean;
    std::string _maketree_clean;
    std::string _command_surround;

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
    const static std::string default_list_dir;
    const static std::string default_list_makefile;
    const static std::string default_cd;
    const static std::string default_command_separator;
    const static std::string default_make;
    const static std::string default_maketree;
    const static std::string default_make_clean;
    const static std::string default_maketree_clean;
    const static std::string default_command_surround;

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
    return_value(list_dir);
    return_value(list_makefile);
    return_value(cd);
    return_value(command_separator);
    return_value(make);
    return_value(maketree);
    return_value(make_clean);
    return_value(maketree_clean);
    return_value(command_surround);
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

