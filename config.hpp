
#if 0
#include <boost/filesystem/path.hpp>
#endif
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;
#if 0
using namespace boost::filesystem;
#endif

class config
{
private:
    void read_file(const string& file)
    {
        ifstream is(file, ifstream::in);
        if(is)
        {
            do
            {
                string i;
                is >> i;
                if(!i.empty())
                {
                    size_t p = i.find(' ');
                    if(p != string::npos)
                    {
                        string k, v;
                        k = i.substr(0, p);
                        v = i.substr(p + 1);
                        if(k == "cc_m")
                            _cc_m = v;
                        else if(k == "cc")
                            _cc = v;
                        else if(k == "list")
                            _list = v;
                    }
                }
            }
            while(!is.eof());
        }
    }

    string _cc_m;
    string _cc;
    string _list;

    const string default_cc_m = "g++ -M";
    const string default_cc = "g++";
    const string default_list = "ls -1 *.cpp *.hpp";

public:
#define return_value(x) \
    const string& x() const { \
        return (_##x.empty() ? default_##x : _##x); }
    return_value(cc_m);
    return_value(cc);
    return_value(list);
#undef return_value

    config()
    {
        const char* autocompile = "autocompile";
#if 0
        path file_path(string(".") + autocompile);
#endif
        if(getenv("HOMEPATH") != nullptr)
        {
            read_file(string(getenv("HOMEPATH")) + "/" + "." + autocompile);
#if 0
            path p(getenv("HOMEPATH"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        if(getenv("HOME") != nullptr)
        {
            read_file(string(getenv("HOME")) + "/" + "." + autocompile);
#if 0
            path p(getenv("HOME"));
            p /= file_path;
            read_file(string(p.native().begin(), p.native().end()));
#endif
        }
        read_file(autocompile);
    }
};

