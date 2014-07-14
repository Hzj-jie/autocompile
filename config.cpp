
#include "config.hpp"
#include <string>
using namespace std;

const string config_t::default_cc_m = "g++ -MM %1%";
const string config_t::default_cc_c = "g++ -c %1% -o %2%";
const string config_t::default_cc = "g++ %1% -o %2%";
const string config_t::default_cc_flag = "-std=c++11 -Wall -Wno-unused-function -Wno-unused-variable -O3";
const string config_t::default_dlink = "-l%1%";
const string config_t::default_list = "ls -1 *.cpp *.c";
const string config_t::default_main = "main.cpp";
const string config_t::default_objs = "";
const string config_t::default_out = "main";
const string config_t::default_dlibs = "";
const string config_t::default_rm = "rm";
const string config_t::default_list_dir = "find . -maxdepth 1 -mindepth 1 -type d -printf %P\\\\n";
const string config_t::default_list_makefile = "ls -1 Makefile makefile GNUmakefile";
const string config_t::default_cd = "cd %1%";
const string config_t::default_command_separator = "%1% ; %2%";
const string config_t::default_make = "make";
const string config_t::default_maketree = "maketree";
const string config_t::default_command_surround = "(%1%)";
const string config_t::default_autocompile = "autocompile > Makefile";
const config_t config_t::instance;

