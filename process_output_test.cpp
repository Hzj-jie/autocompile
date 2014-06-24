
#include "process_output.hpp"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
    cout << "system available " << (system_available ? "yes" : "no") << endl;
    while(1)
    {
        cout << "input a command" << endl;
        string s;
        if(getline(cin ,s))
        {
            vector<string> o;
            vector<string> e;
            cout << "process_output " << s << " " << (process_output(s, o, e) ? "true" : "false") << endl;
            for(int i = 0; i < o.size(); i++)
                cout << "output: " << i << " " << o[i] << endl;
            for(int i = 0; i < e.size(); i++)
                cout << "error: " << i << " " << e[i] << endl;
        }
        else break;
    }
}

