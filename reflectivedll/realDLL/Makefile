CC=gcc 
CXX=g++
CFLAGS=-municode -static
LDFLAGS=-lWinmm -lwinhttp
DLL=-shared

ifeq ($(OS),Windows_NT)
	RM=powershell /c rm
else
	RM=rm
endif


all: maliciousdll.dll testmDll


maliciousdll.dll: 
	$(CXX)  infoTest.cpp httpClient.cpp maliciousdll.cpp  $(DLL) $(CFLAGS) $(LDFLAGS) -o maliciousdll.dll

testmDll: maliciousdll.dll
	$(CXX) test_mdll.cpp -o testmDll.exe