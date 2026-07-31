#include "process_output.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
using namespace std;

int main()
{
    // Run autocompile command in test_project folder
    vector<string> out;
    vector<string> err;
    bool res = process_output("(cd test_project && ../autocompile)", out, err);
    assert(res);
    assert(!out.empty());

    bool found_all = false;
    bool found_clean = false;
    bool found_autocompile = false;

    for(const auto& line : out)
    {
        if(line.find("all:") == 0) found_all = true;
        if(line.find("clean:") == 0) found_clean = true;
        if(line.find("autocompile:") == 0) found_autocompile = true;
    }

    assert(found_all);
    assert(found_clean);
    assert(found_autocompile);

    cout << "autocompile_test passed!" << endl;
    return 0;
}
