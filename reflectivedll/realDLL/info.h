#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <winreg.h>
#include <WinBase.h>
#include <lmcons.h>
#include <securitybaseapi.h>
#include <winerror.h>
#include <iphlpapi.h> 
#include <iptypes.h>
#include <processthreadsapi.h>


#include <ws2tcpip.h>

//gather info (Situational Awareness) 

//Root privileges
//https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership
wchar_t *checkPrivileges();

//Computer Name
wchar_t *myGetComputerName();

//Machine Guid
wchar_t *GetMachineGuid ();

//Current Username
wchar_t *myGetUserName();








/*
//Root privileges?
//https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership
extern wchar_t *checkPrivileges(){
	//Check if admin
	PSID AdministratorsGroup;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	BOOL isAdmin = AllocateAndInitializeSid(&NtAuthority,2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
	0, 0, 0, 0, 0, 0, &AdministratorsGroup );
	
	if(isAdmin){
		if(!CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin )){
			isAdmin = FALSE;
			FreeSid(AdministratorsGroup);
			//std::wcout << L"NOT admin group" << std::endl;
			wchar_t *privilegeGroup = L"NotAdmin";
			return  privilegeGroup;
		}
		FreeSid(AdministratorsGroup);
	}
	wchar_t *privilegeGroup = L"Admin";
	//std::wcout << L"Is Admin group? " << privilegeGroup<< std::endl;
	return  privilegeGroup;
}

//Computer Name
extern wchar_t *myGetComputerName(){
	wchar_t *compNameBuffer = new wchar_t [MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1];
	DWORD BufferSize = MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1;
	long compNameSuccess = GetComputerNameW(compNameBuffer, &BufferSize);
	if(compNameSuccess == 0){
		//std::wcout << L"Computer Name FAILED" << std::endl;
		//std::wcout << L"BufferSize is: " << BufferSize << std::endl;
		//std::wcout << L"Computer Name is" << compNameBuffer << std::endl;
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	else {
		//std::wcout << L"SUCCESS!" <<std::endl;
		//std::wcout << L"The Computer Name is " << compNameBuffer << std::endl;
		return compNameBuffer; 
	}
}

//Machine Guid
extern wchar_t *GetMachineGuid (){
	wchar_t *guidBuffer = new wchar_t[255];  
	DWORD Buffersize = 255;
	HKEY hkey;
	
	//use regopen first
	long regOpenSuccess = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hkey);
	if(regOpenSuccess != ERROR_SUCCESS){
		//std::wcout << L"Reg open FAILED " << GetLastError() << std::endl;
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	//RegGetValueW
	long guidSuccess = RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", L"MachineGuid", RRF_RT_REG_SZ, NULL, guidBuffer, (LPDWORD) &Buffersize);
	if (guidSuccess!= 0){ 
		//std::wcout << L"MACHINE GUID FAILED " << GetLastError() << std::endl;
		wchar_t *FAIL = L"FAIL"; 

		//RegCloseKey	
		RegCloseKey(hkey);
		return FAIL;
	}
	else {
		//std::wcout << L"SUCCESS!" <<std::endl;
		//std::wcout << L"The Machine Guid is " << guidBuffer << std::endl;
		return guidBuffer; 
	}
	//RegCloseKey	
	RegCloseKey(hkey); 
}

//Current Username
extern wchar_t *myGetUserName(){
	wchar_t *userNameBuffer = new wchar_t[UNLEN*sizeof(wchar_t) + 1]; 
	DWORD BufferSize = sizeof(userNameBuffer);
	long UNSuccess = GetUserNameW(userNameBuffer, &BufferSize);
	if (UNSuccess == 0){
			//std::wcout << L"CURRENT USERNAME FAILED" << std::endl;
			wchar_t *FAIL = L"FAIL";
		return FAIL;  
	}
	else {
		//std::wcout << L"SUCCESS!" <<std::endl;
		//std::wcout << L"The current username is " << userNameBuffer << std::endl;
		return userNameBuffer; 
	}
}

*/