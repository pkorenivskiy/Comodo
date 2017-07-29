// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <atlfile.h>

#include "scanlib.h"
#include "../include/MalwareDefs.h"

#define DBFILENAME	L"db.txt"

void init(HMODULE hModule);
void deInit();
void str2byte(const CString& data, unsigned char** pOut);


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: init(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH: deInit();
		break;
	default:
		break;
	}
	return TRUE;
}

void init(HMODULE hModule)
{
	CAtlFile dbFile;
	
	wchar_t lpwszPath[MAX_PATH];
	GetModuleFileName(hModule, lpwszPath, MAX_PATH);
	CString dbFileName(lpwszPath);
	auto idx = dbFileName.ReverseFind(L'\\');
	auto len = dbFileName.GetLength();
	dbFileName.Delete(dbFileName.ReverseFind(L'\\') + 1, dbFileName.GetLength() - dbFileName.ReverseFind(L'\\')  - 1);
	dbFileName.Append(DBFILENAME);

	auto hr = dbFile.Create(dbFileName,
		GENERIC_READ,
		0,
		OPEN_EXISTING);

	if (hr != S_OK)
		return;

	CString dbContent;
	ULONGLONG ullDbSize = 0;
	dbFile.GetSize(ullDbSize);
	char* pBuff = new char[(int)ullDbSize + 1];
	DWORD nByteRead = 0;
	dbFile.Read(pBuff, (DWORD)ullDbSize, nByteRead);
	pBuff[nByteRead] = 0;
	dbContent = CA2T(pBuff);
	delete[] pBuff;
	dbFile.Close();

	int nLinePos = 0;	
	CString sLine = dbContent.Tokenize(L"\r\n", nLinePos);
	while (sLine.IsEmpty() == false)
	{
		int nDotPos = 0;
		CString sDef = sLine.Tokenize(L".", nDotPos);
		while (sDef.IsEmpty() == false)
		{
			TMalware malware;
			malware.HexString = sDef;
			str2byte(sDef, &malware.HexData);
			malware.DataLen = sDef.GetLength() / 2;
			sDef = sLine.Tokenize(L".", nDotPos);
			malware.GUID = sDef;

			g_Malwares.Add(malware);
			break;
		}
		sLine = dbContent.Tokenize(L"\r\n", nLinePos);
	}
}

void deInit()
{

}

void str2byte(const CString& data, unsigned char** pOut) 
{
	*pOut = new unsigned char[data.GetLength() / 2];
	unsigned char* p = *pOut;

	wchar_t b[3];
	b[2] = 0;

	for (auto s = (wchar_t*)data.GetString(); *s; s += 2)
	{
		memcpy(b, s, 4);
		*p = (unsigned char)wcstoul(b, nullptr, 16);
		p++;
	}
}