// scanlib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <atlfile.h>

#include "scanlib.h"

SCANLIB_API CAtlArray<TMalware> g_Malwares;

void str2byte(const CString& data, unsigned char** pOut);

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

SCANLIB_API HRESULT LoadSignatures(const wchar_t* lpwszPath)
{
	CAtlFile dbFile;

	HRESULT hr = dbFile.Create(lpwszPath,
		GENERIC_READ,
		0,
		OPEN_EXISTING);

	if (hr != S_OK)
		return hr;

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

	g_Malwares.RemoveAll();

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

	return S_OK;
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
