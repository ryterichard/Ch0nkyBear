#include <windows.h>
#include <string>
#include <iostream>
#include <winhttp.h>

#include "http.h"

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

std::wstring makeHttpPOSTRequest(std::wstring fqdn, int port, std::wstring uri, bool useTLS, std::wstring payload){
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
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
        bResults = ::WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
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
    return result;
}
/*
int wmain(int argc,  wchar_t* argv[]){
    if(argc !=7){
        std::wcout << L"Incorrect number of arguments: you need 4 positional arguemts" << std::endl;
        return 0;
    }

    std::wstring fqdn = std::wstring(argv[1]);
    int port = std::stoi( argv[2] );
    std::wstring uri = std::wstring(argv[3]);
    int  useTLS =std::stoi(argv[4]);
    bool tls;
    std::wstring payload = std::wstring(argv[5]);
    DWORD payloadLength = std::stoi(argv[6]);
    if (useTLS == 1){
        tls = true;
    } else if (useTLS == 0){
        tls = false;

    } else{
        std::wcout << L"bad value for useTls" << std::endl;
        return 0;
    }
     std::wcout << makeHttpRequest(fqdn,  port, uri, tls, payload, payloadLength) << std::endl;
    return 0;
    
} */