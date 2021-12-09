CC=zig cc
CXX=g++
CFLAGS=-target x86_64-windows-gnu 
LDFLAGS= -municode -static
PDB=-g
BIN=bin

all: infoTest

infoTest:
	$(CXX) infoTest.cpp  $(LDFLAGS)   -o infoTest.exe 

clean:
	rm *.exe