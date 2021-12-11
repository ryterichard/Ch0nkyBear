CC=gcc 
CXX=g++
CFLAGS=-municode
LDFLAGS=-lWinmm
DLL=-shared

ifeq ($(OS),Windows_NT)
	RM=powershell /c rm
else
	RM=rm
endif


all: malicious.dll testmDll.exe


maliciousdll.dll: 
	$(CXX)  maliciousdll.cpp   $(DLL) $(CFLAGS) $(LDFLAGS) -o maliciousdll.dll

testmDll: maliciousdll.dll
	$(CXX) test_mdll.cpp -o testmDll.exe