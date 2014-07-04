
#!/bin/sh

g++ -Wno-unused-function -O3 -c config.cpp -o config.o -std=c++0x -Wall
g++ -Wno-unused-function -O3 -c process_output.cpp -o process_output.o -std=c++0x -Wall
g++ -Wno-unused-function -O3 config_test.cpp -o config_test -std=c++0x config.o -Wall
g++ -Wno-unused-function -O3 process_output_test.cpp -o process_output_test -std=c++0x process_output.o -Wall
g++ -Wno-unused-function -O3 autocompile.cpp -o autocompile -std=c++0x process_output.o config.o -Wall
g++ -Wno-unused-function -O3 maketree.cpp -o maketree -std=c++0x process_output.o config.o -Wall

