#pragma once
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

static std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

static std::string& rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

static std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

static void split(const std::string& s, std::vector<std::string>& o, const char* const delims)
{
    o.clear();
    if(s.empty()) return;
    std::string delim_str(delims ? delims : "");
    size_t start = s.find_first_not_of(delim_str);
    while(start != std::string::npos)
    {
        size_t end = s.find_first_of(delim_str, start);
        if(end == std::string::npos)
        {
            o.push_back(s.substr(start));
            break;
        }
        else
        {
            o.push_back(s.substr(start, end - start));
            start = s.find_first_not_of(delim_str, end);
        }
    }
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
        if(p == std::string::npos)
        {
            first = s;
            second = std::string();
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
    std::istringstream convert(s);
    return (convert >> o);
}

// Minimal drop-in replacement for boost::format (%1%, %2%, etc.)
class format
{
private:
    std::string s;
    int index;

public:
    explicit format(const std::string& fmt) : s(fmt), index(1) {}

    template <typename T>
    format& operator%(const T& val)
    {
        std::string placeholder = "%" + std::to_string(index++) + "%";
        size_t pos = s.find(placeholder);
        if (pos != std::string::npos)
        {
            std::ostringstream ss;
            ss << val;
            s.replace(pos, placeholder.length(), ss.str());
        }
        return *this;
    }

    std::string str() const
    {
        return s;
    }

    friend std::ostream& operator<<(std::ostream& os, const format& f)
    {
        return os << f.s;
    }
};

static inline std::string str(const format& f)
{
    return f.str();
}
