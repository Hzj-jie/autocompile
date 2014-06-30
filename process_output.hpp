
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "file.hpp"

struct system_available_t
{
private:
    bool v;
public:
    operator bool() const
    {
        return v;
    }

    system_available_t()
    {
        v = (system(nullptr) != 0);
    }
} system_available;

static bool process_output(const std::string& cmd, std::vector<std::string>& output, std::vector<std::string>& error)
{
    using namespace std;
    const char* out = "temp.out";
    const char* err = "temp.err";
    if(system_available)
    {
        int i = system((cmd + " 1>> " + out + " 2>> " + err).c_str());
        bool r = (i != 127) &&
                 (i != 9009);
        r = read_all_lines(string(out), output) && r;
        r = read_all_lines(string(err), error) && r;
        r = (remove(out) == 0) && r;
        r = (remove(err) == 0) && r;
        return r;
    }
    else return false;
}

