
#pragma once
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <boost/tokenizer.hpp>
#include <sstream>

static std::string& ltrim(std::string& s)
{
    using namespace std;
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

static std::string& rtrim(std::string& s)
{
    using namespace std;
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

static std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

static void split(const std::string& s, std::vector<std::string>& o, const char* const delims)
{
    using namespace boost;
    o.clear();
    tokenizer<char_separator<char> > tokens(s, char_separator<char>(delims));
    for(auto it = tokens.begin(); it != tokens.end(); it++)
        o.push_back(*it);
}

static void split(const std::string& s, std::vector<std::string>& o, const std::string& delims)
{
    split(s, o, delims.c_str());
}

static void split(const std::string& s, std::vector<std::string>& o)
{
    split(s, o, "\t,; ");
}

static bool split(const std::string& s, std::string& first, std::string& second, const char* const delims)
{
    if(s.empty()) return false;
    else
    {
        size_t p = s.find_first_of(delims);
        if(p == string::npos)
        {
            first = s;
            second = string();
            return false;
        }
        else
        {
            first = s.substr(0, p);
            second = s.substr(p + 1);
            return true;
        }
    }
}

static bool split(const std::string& s, std::string& first, std::string& second, const std::string& delims)
{
    return split(s, first, second, delims.c_str());
}

static bool split(const std::string& s, std::string& first, std::string& second)
{
    return split(s, first, second, " \t");
}

template <typename T>
static bool from_str(const std::string& s, T& o)
{
    istreamstream convert(s);
    return (convert >> o);
}

