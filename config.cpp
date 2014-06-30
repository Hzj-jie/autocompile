
#include "config.hpp"
#include <string>
using namespace std;

const string config_t::default_cc_m = "g++ -M";
const string config_t::default_cc_c = "g++ -c %1% -o %2%";
const string config_t::default_cc = "g++ %1% -o %2%";
const string config_t::default_cc_flag = "";
const string config_t::default_dlink = "-l";
const string config_t::default_list = "ls -1 *.cpp *.c";
const string config_t::default_main = "main.cpp";
const string config_t::default_objs = "";
const string config_t::default_out = "main";
const string config_t::default_dlibs = "";
const string config_t::default_rm = "rm";
const config_t config_t::instance;

