#pragma once

#include <atlcoll.h>

#include "../include/MalwareDefs.h"

#ifdef SCANLIB_EXPORTS
#define SCANLIB_API __declspec(dllexport)
#else
#define SCANLIB_API __declspec(dllimport)
#endif

extern SCANLIB_API CAtlArray<TMalware> g_Malwares;

SCANLIB_API HRESULT ScanFile(const wchar_t* lpwszPath, TScanResult** ppMalwares, size_t& count);
SCANLIB_API HRESULT ScanArray(const unsigned char* lpArray, const unsigned long nLength, TScanResult** ppMalwares, size_t& count);
SCANLIB_API HRESULT GetFileSignature(const wchar_t* lpwszPath, CAtlArray<CString>& malwares);
