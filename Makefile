CC = g++
LDFLAGS = -g -O0 -Wno-write-strings
CXXFLAGS = -g -O0 -Wno-write-strings


ImagenES.o: ImagenES.h ImagenES.cpp
	g++ -c ImagenES.cpp ImagenES.h

calcDWT.o: calcDWT.h calcDWT.cpp
	g++ -c calcDWT.h calcDWT.cpp

dwt.o: ImagenES.o calcDWT.o
	g++ -c dwt.cpp ImagenES.o calcDWT.o	
all: dwt.o ImagenES.o calcDWT.o
	g++ dwt.o ImagenES.o calcDWT.o -o dwt


clean: 
	rm -rf *o dwt
