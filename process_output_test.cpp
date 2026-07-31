#include "process_output.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

int main()
{
    // Test 1: Verify system availability
    assert(system_available);

    // Test 2: Standard output capture
    {
        vector<string> out;
        vector<string> err;
        bool res = process_output("echo hello_world", out, err);
        assert(res);
        assert(!out.empty());
        assert(out[0] == "hello_world");
    }

    // Test 3: Standard error capture
    {
        vector<string> out;
        vector<string> err;
        bool res = process_output("(echo error_test >&2)", out, err);
        assert(res);
        assert(!err.empty());
        assert(err[0] == "error_test");
    }

    // Test 4: Failed command handling
    {
        vector<string> out;
        vector<string> err;
        bool res = process_output("non_existent_command_xyz12345", out, err);
        assert(!res);
    }

    cout << "process_output_test passed!" << endl;
    return 0;
}
