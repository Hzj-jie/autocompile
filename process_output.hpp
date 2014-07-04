
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include "file.hpp"

const static struct system_available_t
{
private:
    bool v;
public:
    operator bool() const
    {
        return v;
    }

    static const system_available_t instance;

private:
    system_available_t()
    {
        v = (system(nullptr) != 0);
    }
} &system_available = system_available_t::instance;

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

static bool process_output(const std::string& s,
                           std::vector<std::string>& out,
                           std::mutex* mtx = nullptr)
{
    using namespace std;
    vector<string> err;
    if(process_output(s, out, err))
    {
        if(!err.empty())
        {
            if(mtx != nullptr) mtx->lock();
            cerr << "error detected when running " << s << endl;
            for(int i = 0; i < err.size(); i++)
            {
                cerr << "l(" << i << ") " << err[i] << endl;
            }
            cerr << "usually this does not show a problem, just as a reference" << endl;
            if(mtx != nullptr) mtx->unlock();
        }
        return true;
    }
    else
    {
        if(mtx != nullptr) mtx->lock();
        cerr << "failed to run command " << s << endl;
        if(mtx != nullptr) mtx->unlock();
        return false;
    }
}

