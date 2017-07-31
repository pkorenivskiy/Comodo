#pragma once

#include <atlcoll.h>

#include "../include/MalwareDefs.h"

#ifdef SCANLIB_EXPORTS
#define SCANLIB_API __declspec(dllexport)
#else
#define SCANLIB_API __declspec(dllimport)
#endif

namespace scandll
{
	extern SCANLIB_API CAtlArray<TMalware> g_Malwares;

	SCANLIB_API HRESULT ScanFile(const wchar_t* lpwszPath, wchar_t** ppGuid, long* pnOffset);
	SCANLIB_API HRESULT ScanArray(const unsigned char* lpArray, const unsigned long nLength, wchar_t** ppGuid, long* pnOffset);
	SCANLIB_API HRESULT LoadSignatures(const wchar_t* lpwszPath);
}