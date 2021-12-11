#include <windows.h>
#include <string>
#include <iostream>
#include <winhttp.h>

std::wstring makeHttpREGISTERRequest(std::wstring fqdn, int port, std::wstring uri, bool useTLS, std::wstring payload, DWORD payloadLength);

std::wstring makeHttpPOSTRequest(std::wstring fqdn, int port, std::wstring uri, bool useTLS, std::wstring payload, DWORD payloadLength);