
#!/bin/sh

g++ -c config.cpp -o config.o -std=c++0x
g++ -c process_output.cpp -o process_output.o -std=c++0x
g++ config_test.cpp -o config_test -std=c++0x config.o
g++ process_output_test.cpp -o process_output_test -std=c++0x process_output.o
g++ autocompile.cpp -o autocompile -std=c++0x process_output.o config.o

