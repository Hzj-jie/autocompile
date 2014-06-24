
#include <vector>
#include <string>
#include <iostream>
#include "config.hpp"
#include "process_output.hpp"
using namespace std;

void print_error(const vector<string>& err)
{
    if(!err.empty())
    {
        cerr << "error detected when running " << config.list() << endl;
        for(int i = 0; i < err.size(); i++)
        {
            cerr << "l(" << i << ") " << err[i] << endl;
        }
        cerr << "usually this does not show a problem, just as a reference" << endl;
    }
}

int main()
{
#define failed_to_run_command(x) { \
    cerr << "failed to run command " << x << endl; \
    return -1; }
    vector<string> out;
    vector<string> err;
    if(process_output(config.list(), out, err))
    {
        vector<string> files = out;
        print_error(err);
        bool has_main = false;
        for(int i = 0; i < files.size(); i++)
        {
            if(out[i] == config.main())
            {
                has_main = true;
            }
            else
            {
                if(process_output(config.cc_m() + " " + files[i], out, err))
                {
                    print_error(err);
                    for(int j = 0; j < out.size(); j++)
                        cout << out[j] << endl;
                }
                else
                {
                    failed_to_run_command(config.cc_m() + " " + files[i]);
                }
            }
        }
    }
    else
    {
        failed_to_run_command(config.list());
    }
#undef failed_to_run_command
}

