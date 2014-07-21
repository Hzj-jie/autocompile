
#include <vector>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include "config.hpp"
#include "process_output.hpp"
#include "strutils.hpp"
using namespace std;
using namespace boost;

static const int RUN_COMMAND_FAILURE = -1;

static void append_if_not_empty(string& s, const string& c)
{
    if(!c.empty())
    {
        s.push_back(' ');
        s.append(c);
    }
}

static void append_if_not_empty(ostream& os, const string& c)
{
    if(!c.empty()) os << ' ' << c;
}

template <typename T>
static void append_cc_flag(T& s)
{
    append_if_not_empty(s, config.cc_flag());
    append_if_not_empty(s, config.cc_flag2());
}

static bool print_dependence(const string& file,
                             const string& replace_target_from,
                             const string& replace_target_to,
                             bool nl_last)
{
    vector<string> out;
    string cmd = (format(config.cc_m()) % file).str();
    append_cc_flag(cmd);
    if(process_output(cmd, out) &&
       !out.empty())
    {
        if(!replace_target_from.empty() &&
           !replace_target_to.empty())
        {
            size_t i = out[0].find(replace_target_from);
            if(i != string::npos)
            {
                out[0].replace(i, replace_target_from.length(), replace_target_to);
            }
        }
        for(size_t j = 0; j < out.size(); j++)
        {
            cout << out[j];
            if(nl_last || j < out.size() - 1)
                cout << endl;
        }
        return true;
    }
    else return false;
}

static bool print_dependence(const string& file)
{
    return print_dependence(file, string(), string(), true);
}

void append_link(string& s, const string& o)
{
    s.push_back(' ');
    s.append(o);
}

void append_dlink(string& s, const string& o)
{
    s.push_back(' ');
    s.append((format(config.dlink()) % o).str());
}

string to_obj(const string& file)
{
    return replace_suffix(file, "o");
}

int main()
{
    vector<string> files;
    if(process_output(config.list(), files))
    {
        cout << endl
             << "# this make file is generated by autocompile,"
             << endl
             << "# with .autocompile configuration file,"
             << endl
             << "# so change the configuration file or autocompile logic is prefered"
             << endl
             << endl;
        cout << "all:";
        for(size_t i = 0; i < files.size(); i++)
        {
            cout << " \\" << endl << " ";
            if(files[i] == config.main())
                cout << config.out();
            else
                cout << to_obj(files[i]);
        }
        cout << endl << endl;

        bool has_main = false;
        for(size_t i = 0; i < files.size(); i++)
        {
            if(files[i] != config.main())
            {
                if(print_dependence(files[i]))
                {
                    cout << '\t'
                         << format(config.cc_c()) % files[i] % to_obj(files[i]);
                    append_cc_flag(cout);
                    cout << endl << endl;
                }
                else return RUN_COMMAND_FAILURE;
            }
            else has_main = true;
        }
        if(has_main)
        {
            if(print_dependence(config.main(), to_obj(config.main()), config.out(), false))
            {
                vector<string> deps;
                for(size_t i = 0; i < files.size(); i++)
                {
                    if(files[i] != config.main())
                        deps.push_back(to_obj(files[i]));
                }
                for(size_t i = 0; i < deps.size(); i++)
                    cout << " \\" << endl << ' ' << deps[i];
                vector<string> objs;
                split(config.objs(), objs);
                for(size_t i = 0; i < objs.size(); i++)
                    cout << " \\" << endl << ' ' << objs[i];

                string o = str(format(config.cc()) % config.main() % config.out());
                append_cc_flag(o);

                vector<string> dlibs;
                split(config.dlibs(), dlibs);
                for(size_t i = 0; i < dlibs.size(); i++)
                    append_dlink(o, dlibs[i]);
                for(size_t i = 0; i < objs.size(); i++)
                    append_link(o, objs[i]);
                for(size_t i = 0; i < deps.size(); i++)
                    append_link(o, deps[i]);
                cout << endl << '\t' << o << endl << endl;
            }
            else return RUN_COMMAND_FAILURE;
        }

        cout << "clean:" << endl;
        cout << "\t-" << config.rm();
        for(size_t i = 0; i < files.size(); i++)
        {
            cout << ' ';
            if(files[i] == config.main())
                cout << config.out();
            else
                cout << to_obj(files[i]);
        }
        cout << endl << endl;

        cout << "autocompile:" << endl;
        cout << '\t' << config.autocompile();
        cout << endl << endl;
    }
    else return RUN_COMMAND_FAILURE;
}

