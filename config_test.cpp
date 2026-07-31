#include "config.hpp"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    const config_t& c = config;

    // Verify non-empty default configurations
    assert(!c.cc_m().empty());
    assert(!c.cc_c().empty());
    assert(!c.cc_h().empty());
    assert(!c.cc().empty());
    assert(!c.cc_flag().empty());
    assert(!c.dlink().empty());
    assert(!c.list().empty());
    assert(!c.list_h().empty());
    assert(!c.main().empty());
    assert(!c.out().empty());
    assert(!c.rm().empty());
    assert(!c.list_dir().empty());
    assert(!c.list_makefile().empty());
    assert(!c.cd().empty());
    assert(!c.command_separator().empty());
    assert(!c.make().empty());
    assert(!c.maketree().empty());
    assert(!c.command_surround().empty());
    assert(!c.autocompile().empty());

    // Verify specific default string patterns
    assert(c.main() == "main.cpp");
    assert(c.out() == "main");
    assert(c.rm() == "rm");
    assert(c.make() == "make");
    assert(c.dlink() == "-l%1%");

    cout << "config_test passed!" << endl;
    return 0;
}
