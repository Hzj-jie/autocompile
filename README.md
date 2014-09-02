autocompile
===========

a tool set to help generate makefile and build a tree structure

autocompile tool set is targeting to manage a small or middle size project, free developpers from writting and updating makefiles, and build project tree from top level.
autocompile tool set does not replace make or compiler, instead it generate makefile for make, and use compiler to detect source file dependencies.

this tool set contains two tools,

1. autocompile

it goes through current directory to looking for .c and .cpp files, generate dependencies according to g++ -MM output, and output makefile to stdout.

autocompile looks for .autocompile from $HOMEPATH/.autocompile and ./.autocompile as configuration

2. maketree

it goes through current directory to looking for sub folders to compile, if a subfolder contains Makefile / makefile / GNUmakefile, it will call make, otherwise call maketree

support command line parameters
  - -1 use only one thread
  - -v output the commands to run, instead of run them
  - other command line parameters will be sent to make or child maketree process. say 'maketree autocompile' will run 'make autocompile' if Makefile exists, and 'maketree autocompile'.

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

  - cc-h

    the g++ -x c++-header as boost::format string, default value is g++ -x c++-header %1% -o %2%

    alias cc_h, compile-header, compile_header

  - cc

    the g++ command as boost::format string, default value is g++ %1% -o %2%

    alias compile-link

  - all-cmd

    the make command to run in 'all' target, the 'all' target is a phony target by default, but you can provide a command for it. default value is empty.

    alias all_cmd

  - cc-flag

    extra g++ options, default value is -std=c++11 -Wall -Wno-unused-function -Wno-unused-variable -O3

    alias cc_flag, compiler-flag, compiler-option

  - cc-flag2

    extra g++ options, default value is empty, so you can change the compiling command but inherits the default options cc-flag provided.

    alias cc_flag2, compiler-flag2, compiler-option2, extra-compiler-flag, extra-compiler-option

  - dlink

    the dynamic linking command for the compiler as boost::format string, default value is -l%1%

    alias dynamic-link

  - list

    the system command to list .c and .cpp file, default value is ls -1 *.cpp *.c

  - list-h

    the system command to list .h and .hpp file, default value is ls -1 *.hpp *.h

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


examples,

there are two example projects in the folder

  - test-project

    it shows how to use autocompile to generate Makefile, it has one .h file, one .hpp file, two .cpp files, while it also needs to include boost_system to compile. so the .autocompile file only needs to provide the dlibs as boost_system.

    run ./autocompile.sh or ../autocompile > Makefile will generate Makefile against .autocompile.

  - test-project2

    it shows the combination of maketree and autocompile, while maketree can help to update Makefile by running 'maketree autocompile'. it will run 'make autocompile' in each folder with Makefile.

    while there are several dirs files to show the dependencies between each folder.


howto,

  - change compiler

    if you would like to change compiler for a project, but do not want to change .autocompile files from all the folders, you can choose to create a .autocompile file in home folder, i.e. /home/<user-name>/.autocompile, and set cc parameter. i.e. 'cc clang++' in your .autocompile file. and run 'maketree autocompile' to update all the Makefiles in the project.

