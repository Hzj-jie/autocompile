
#!/bin/sh

g++ -Wno-unused-function -c config.cpp -o config.o -std=c++0x -Wall -pthread
g++ -Wno-unused-function -c process_output.cpp -o process_output.o -std=c++0x -Wall -pthread
g++ -Wno-unused-function config_test.cpp -o config_test -std=c++0x config.o -Wall -pthread
g++ -Wno-unused-function process_output_test.cpp -o process_output_test -std=c++0x process_output.o -Wall -pthread
g++ -Wno-unused-function autocompile.cpp -o autocompile -std=c++0x process_output.o config.o -Wall -pthread
g++ -Wno-unused-function maketree.cpp -o maketree -std=c++0x process_output.o config.o -Wall -pthread

