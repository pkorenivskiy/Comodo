// scanlib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <atlfile.h>

#include "scanlib.h"

SCANLIB_API CAtlArray<TMalware> g_Malwares;

SCANLIB_API HRESULT ScanFile(const wchar_t * lpwszPath, TScanResult** ppMalwares, size_t& count)
{
	CAtlFile file;
	CAtlFileMappingBase mapFile;

	HRESULT hr = file.Create(lpwszPath,
		GENERIC_READ,
		0,
		OPEN_EXISTING);

	HANDLE hFile = (HANDLE)file;
	if (hr != S_OK || hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	if (mapFile.MapFile(hFile) != S_OK)
	{
		file.Close();
		return E_FAIL;
	}

	auto lpData = (unsigned char*)mapFile.GetData();
	auto nSize = mapFile.GetMappingSize();

	hr = ScanArray(lpData, nSize, ppMalwares, count);

	mapFile.Unmap();
	file.Close();

	return hr;
}

SCANLIB_API HRESULT ScanArray(const unsigned char * lpArray, const unsigned long nLength, TScanResult** ppMalwares, size_t& count)
{
	count = 0;
	CAtlMap<SIZE_T, TMalware> malwares;
	
	for (unsigned long i = 0; i < nLength; ++i)
	{
		for (size_t y = 0; y < g_Malwares.GetCount(); ++y)
		{
			if (nLength - i > g_Malwares[y].DataLen && memcmp(lpArray + i, g_Malwares[y].HexData, g_Malwares[y].DataLen) == 0)
			{
				malwares[i] = g_Malwares[y];
			}
		}
	}


	if (malwares.IsEmpty() == false)
	{
		*ppMalwares = new TScanResult[malwares.GetCount()];
		auto pos = malwares.GetStartPosition();		
		while (pos != NULL)
		{
			auto it = malwares.GetNext(pos);
			TScanResult scanResult;
			scanResult.Offset = it->m_key;
			scanResult.Malware = it->m_value;

			*ppMalwares[count++] = scanResult;
		}
	}

	return S_OK;
}

SCANLIB_API HRESULT GetFileSignature(const wchar_t * lpwszPath, CAtlArray<CString>& malwares)
{
	return E_NOTIMPL;
}
