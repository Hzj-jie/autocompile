
#include <vector>
#include <string>
#include <iostream>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "config.hpp"
#include "process_output.hpp"
using namespace std;
using namespace boost;

static const int RUN_COMMAND_FAILURE = -1;

static bool process_output2(const string& s, vector<string>& out)
{
    vector<string> err;
    if(process_output(s, out, err))
    {
        if(!err.empty())
        {
            cerr << "error detected when running " << s << endl;
            for(int i = 0; i < err.size(); i++)
            {
                cerr << "l(" << i << ") " << err[i] << endl;
            }
            cerr << "usually this does not show a problem, just as a reference" << endl;
        }
        return true;
    }
    else
    {
        cerr << "failed to run command " << s << endl;
        return false;
    }
}

static bool print_dependence(const string& file,
                             const string& replace_target_from,
                             const string& replace_target_to,
                             bool nl_last)
{
    vector<string> out;
    if(process_output2(config.cc_m() + " " + file, out) &&
       !out.empty())
    {
        if(!replace_target_from.empty() &&
           !replace_target_to.empty())
        {
            int i = out[0].find(replace_target_from);
            if(i != string::npos)
            {
                out[0].replace(i, replace_target_from.length(), replace_target_to);
            }
        }
        for(int j = 0; j < out.size(); j++)
        {
            cout << out[j];
            if(nl_last || j < out.size() - 1)
                cout << endl;
        }
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
    s.append(config.dlink());
    s.append(o);
}

string to_obj(const string& file)
{
    return replace_suffix(file, "o");
}

void split(const string& s, vector<string>& o)
{
    o.clear();
    tokenizer<char_separator<char> > tokens(s, char_separator<char>(",; "));
    for(auto it = tokens.begin(); it != tokens.end(); it++)
        o.push_back(*it);
}

int main()
{
    vector<string> files;
    if(process_output2(config.list(), files))
    {
        cout << "all:";
        for(int i = 0; i < files.size(); i++)
        {
            cout << " \\" << endl << " ";
            if(files[i] == config.main())
                cout << config.out();
            else
                cout << to_obj(files[i]);
        }
        cout << endl << endl;

        bool has_main = false;
        for(int i = 0; i < files.size(); i++)
        {
            if(files[i] != config.main())
            {
                if(print_dependence(files[i]))
                {
                    cout << '\t'
                         << format(config.cc_c()) % files[i] % to_obj(files[i]);
                    if(!config.cc_flag().empty())
                        cout << ' ' << config.cc_flag();
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
                for(int i = 0; i < files.size(); i++)
                {
                    if(files[i] != config.main())
                        deps.push_back(to_obj(files[i]));
                }
                for(int i = 0; i < deps.size(); i++)
                    cout << " \\" << endl << ' ' << deps[i];
                vector<string> dlibs;
                split(config.dlibs(), dlibs);
                for(int i = 0; i < dlibs.size(); i++)
                    cout << " \\" << endl << ' ' << dlibs[i];

                string o = str(format(config.cc()) % config.main() % config.out());
                if(!config.cc_flag().empty())
                {
                    o.push_back(' ');
                    o.append(config.cc_flag());
                }
                vector<string> objs;
                split(config.objs(), objs);
                for(int i = 0; i < dlibs.size(); i++)
                    append_dlink(o, dlibs[i]);
                for(int i = 0; i < objs.size(); i++)
                    append_link(o, objs[i]);
                for(int i = 0; i < deps.size(); i++)
                    append_link(o, deps[i]);
                cout << endl << '\t' << o << endl << endl;
            }
            else return RUN_COMMAND_FAILURE;
        }

        cout << "clean:" << endl;
        cout << '\t' << config.rm();
        for(int i = 0; i < files.size(); i++)
        {
            cout << ' ';
            if(files[i] == config.main())
                cout << config.out();
            else
                cout << to_obj(files[i]);
        }
        cout << endl << endl;
    }
    else return RUN_COMMAND_FAILURE;
}

