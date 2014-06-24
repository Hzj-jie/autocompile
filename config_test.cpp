
#include "config.hpp"
#include <iostream>
using namespace std;

int main()
{
    const config_t& c = config;
#define p(x) cout << #x << "\t" << c.x() << endl;
    p(cc_m);
    p(cc);
    p(list);
    p(main);
    p(objs);
#undef p
}

