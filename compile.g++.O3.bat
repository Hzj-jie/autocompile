g++ -Wno-unused-function -O3 -c config.cpp -o config.o -std=c++11 -Wall -pthread
g++ -Wno-unused-function -O3 -c process_output.cpp -o process_output.o -std=c++11 -Wall -pthread
g++ -Wno-unused-function -O3 config_test.cpp -o config_test -std=c++11 config.o -Wall -pthread
g++ -Wno-unused-function -O3 process_output_test.cpp -o process_output_test -std=c++11 process_output.o -Wall -pthread
g++ -Wno-unused-function -O3 autocompile.cpp -o autocompile -std=c++11 process_output.o config.o -Wall -pthread
g++ -Wno-unused-function -O3 maketree.cpp -o maketree -std=c++11 process_output.o config.o -Wall -pthread
g++ -Wno-unused-function -O3 autocompile_test.cpp -o autocompile_test -std=c++11 process_output.o config.o -Wall -pthread
g++ -Wno-unused-function -O3 maketree_test.cpp -o maketree_test -std=c++11 process_output.o config.o -Wall -pthread
