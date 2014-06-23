
#include "config.hpp"
#include <iostream>
using namespace std;

int main()
{
    config c;
#define p(x) cout << #x << "\t" << c.x() << endl;
    p(cc_m);
    p(cc);
    p(list);
#undef p
}

