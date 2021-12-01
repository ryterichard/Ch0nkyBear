#include <windows.h>
#include <stdio.h>
#include <iostream>

//DLL stealer


//gather info (Situational Awareness)


//Ability to follow commmands from C2

//Characteristics pre configured??



//EXPORT REFLECTIVE LOADER FUNCTION
//possibly import library that loads for me?


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