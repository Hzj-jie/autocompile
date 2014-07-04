autocompile
===========

a tool set to help generate makefile and build a tree structure

this tool set contains two tools,
1. autocompile
it goes through current directory to looking for .c and .cpp files, generate dependencies according to g++ -M output, and output makefile to stdout.
autocompile looks for .autocompile from $HOMEPATH/.autocompile and ./.autocompile as configuration
2. maketree
it goes through current directory to looking for sub folders to compile, if a subfolder contains Makefile / makefile / GNUmakefile, it will call make, otherwise call maketree
support command line parameters
  - -c make clean instead of make
  - -v output the commands to run, instead of run them
  - an integer, the concurrency, maketree will use at most these threads
maketree also looks for .autocompile from $HOMEPATH/.autocompile and ./.autocompile as configuration
maketree looks for dirs file in current folder to decide the folders ignored, and the dependencies between the folders

usually you do not need to change anything to let autocompile and maketree work in linux or cygwin environments, except for these parameters
in .autocompile file
  - main
  - out
if you need to build a binary instead of object files from a source file, the detail of these parameters are in the follow section
in dirs file
  - folder1:folder2 folder3
if folder1 depends on folder2 and folder3, i.e. folder1 can only be built after folder2 and folder3

.autocompile can include
  - cc-m
    the g++ -M command as boost::format string, default value is g++ -M %1%
    alias cc_m, compiler-dependency, compiler-dependencies, cc_M, cc-M
  - cc-c
    the g++ -c command as boost::format string, default value is g++ -c %1% -o %2%
    alias cc_c, compile
  - cc
    the g++ command as boost::format string, default value is g++ %1% -o %2%
    alias compile-link
  - cc-flag
    extra g++ options, default value is -std=c++11
    alias cc_flag, compiler-flag, compiler-option
  - dlink
    the dynamic linking command for the compiler as boost::format string, default value is -l%1%
    alias dynamic-link
  - list
    the system command to list .c and .cpp file, default value is ls -1 *.cpp *.c
  - main
    the name of the cpp file contains main, if this file is existing, the makefile will compile and link it, default value is main.cpp
  - objs
    the static libs link to the binary generated, separated by ',' ' ' or ';', default value is empty
    alias libs, static-libs
  - out
    the output file name of the binary generated, default value is main
    alias output
  - dlibs
    the dynamic libs link to the binary generated, separated by ',' ' ' or ';', default value is empty
    alias dynamic-libs
  - rm
    the system command to remove several files, default value is rm
    alias remove
  - list-dir
    the system command to list all directories in a folder, default value is find . -maxdepth 1 -mindepth 1 -type d -printf %P\\n
    alias list_dir
  - list-makefile
    the system command to list makefile in a folder, default value is ls -1 Makefile makefile GNUmakefile
    alias list_makefile
  - cd
    the system command to change current directory as boost::format string, default value is cd %1%
  - command-separator
    the system defined character to run several commands serially as boost::format string, default value is %1% ; %2%
    alias command_separator
  - make
    the make command to run makefile, default value is make
  - maketree
    the maketree command to run maketree in a folder, default value is maketree
  - make-clean
    the make clean command to run clean target in a makefile, default value is make clean
    alias make_clean
  - maketree-clean
    the maketree clean command to run in a folder, default value is maketree -c
    alias maketree_clean
  - command-surround
    the system defined method to surround several serial command in boost::format string, default value is (%1%)
    alias command_surround

dirs can include
  - -folder
    remove folder from build targets in current directory
  - folder1:folder2 folder3
    folder1 depends on folder2 and folder3
    you can also write this line as
    folder1 folder2 folder3

