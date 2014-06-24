
#include <vector>
#include <string>
#include <iostream>
#include "config.hpp"
#include "process_output.hpp"
using namespace std;

int main()
{
    vector<string> out;
    vector<string> err;
    if(process_output(config.list(), out, err))
    {
        for(int i = 0; i < out.size(); i++)
            cout << out[i] << endl;
        for(int i = 0; i < err.size(); i++)
            cout << err[i] << endl;
    }
    else cerr << "failed to run command " << config.list() << endl;
}

