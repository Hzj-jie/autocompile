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
    bool found_makefile_rule = false;

    for(const auto& line : out)
    {
        if(line.find("all:") == 0) found_all = true;
        if(line.find("clean:") == 0) found_clean = true;
        if(line.find("autocompile:") == 0) found_autocompile = true;
        if(line.find("Makefile: $(wildcard ") == 0 &&
           line.find(".autocompile)") != string::npos) found_makefile_rule = true;
    }

    assert(found_all);
    assert(found_clean);
    assert(found_autocompile);
    assert(found_makefile_rule);

    // Test incremental Makefile rule with GNU Make
    res = process_output("(rm -rf test_project_inc && "
                         "mkdir -p test_project_inc && "
                         "cp -r test_project/* test_project_inc/ && "
                         "cd test_project_inc && "
                         "PATH=..:$PATH autocompile > Makefile && "
                         "! grep -q 'c.o' Makefile && "
                         "touch c.cpp && "
                         "PATH=..:$PATH make Makefile && "
                         "grep -q 'c.o' Makefile && "
                         "rm -rf test_project_inc)", out, err);
    assert(res);

    cout << "autocompile_test passed!" << endl;
    return 0;
}
