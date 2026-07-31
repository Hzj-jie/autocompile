#include "process_output.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

int main()
{
    // Run maketree -v in dry-run mode inside test_project2 folder
    vector<string> out;
    vector<string> err;
    bool res = process_output("(cd test_project2 && ../maketree -v)", out, err);
    assert(res);
    assert(!out.empty());

    bool found_starts_command = false;
    for(const auto& line : out)
    {
        if(line.find("starts command") != string::npos)
        {
            found_starts_command = true;
            break;
        }
    }
    assert(found_starts_command);

    cout << "maketree_test passed!" << endl;
    return 0;
}
