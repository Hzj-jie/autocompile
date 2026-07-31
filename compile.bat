cl /Ox /Wall /EHsc /c config.cpp /Foconfig.o
cl /Ox /Wall /EHsc /c process_output.cpp /Foprocess_output.o
cl /Ox /Wall /EHsc config_test.cpp /link config.o
cl /Ox /Wall /EHsc process_output_test.cpp /link process_output.o
cl /Ox /Wall /EHsc autocompile.cpp /link config.o process_output.o
cl /Ox /Wall /EHsc maketree.cpp /link config.o process_output.o
cl /Ox /Wall /EHsc autocompile_test.cpp /link config.o process_output.o
cl /Ox /Wall /EHsc maketree_test.cpp /link config.o process_output.o
del *.obj
