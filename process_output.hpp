
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include "file.hpp"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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

static std::string uuid_long_str()
{
    using namespace std;
    using namespace boost::uuids;
    return to_string(random_generator()());
}

static bool process_output(const std::string& cmd, std::vector<std::string>& output, std::vector<std::string>& error)
{
    using namespace std;
    const char* const out = ".out";
    const char* const err = ".err";
    string uuid = uuid_long_str();
    if(system_available)
    {
        int i = system((cmd + " 1>> " + uuid + out + " 2>> " + uuid + err).c_str());
        bool r = (i != 127) &&
                 (i != 9009);
        r = read_all_lines(uuid + out, output) && r;
        r = read_all_lines(uuid + err, error) && r;
        r = (remove((uuid + out).c_str()) == 0) && r;
        r = (remove((uuid + err).c_str()) == 0) && r;
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
            cout << "error detected when running " << s << endl;
            for(size_t i = 0; i < err.size(); i++)
                cout << "l(" << i << ") " << err[i] << endl;
            cout << "usually this does not show a problem, just as a reference" << endl;
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

