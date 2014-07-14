
cl /Ox /Wall /EHsc /c config.cpp /Foconfig.o
cl /Ox /Wall /EHsc /c process_output.cpp /Foprocess_output.o
cl /Ox /Wall /EHsc config_test.cpp /link config.o
cl /Ox /Wall /EHsc process_output_test.cpp /link process_output.o
cl /Ox /Wall /EHsc /Ic:\Boost\include\boost-1_55 autocompile.cpp /link config.o process_output.o
cl /Ox /Wall /EHsc /Ic:\Boost\include\boost-1_55 maketree.cpp /link config.o process_output.o
del *.obj

