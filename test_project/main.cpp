
#include "a.h"
#include "b.hpp"
#include <boost/system/error_code.hpp>

int main()
{
    func1();
    func2();
    func3();
    func4();
    cout << boost::system::get_system_category().name()
         << '\t'
         << boost::system::get_system_category().message(0)
         << endl;
}

