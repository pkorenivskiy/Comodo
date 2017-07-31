// Scanner.cpp : Implementation of CScanner

#include "stdafx.h"

#include <atlsafe.h>

#include "Scanner.h"

#include "../scanlib/scanlib.h"

// CScanner



STDMETHODIMP CScanner::ScanFile(BSTR lpwszPath, long* lOffset, BSTR* lpwszGUID)
{
	wchar_t* pGuid;

	auto hr =  scandll::ScanFile(lpwszPath, &pGuid, lOffset);

	if (hr == S_OK && pGuid != nullptr)
	{
		CComBSTR bstrGuid(pGuid);
		bstrGuid.CopyTo(lpwszGUID);
		delete[] pGuid;
	}

	return S_OK;
}

STDMETHODIMP CScanner::ScanArray(VARIANT pData, long* lOffset, BSTR* lpwszGUID)
{
	CComSafeArray<unsigned char> pSa;
	pSa.Attach(pData.parray);
	void* data;
	SafeArrayAccessData(pSa, &data);
	wchar_t* pGuid;
	auto hr = scandll::ScanArray((unsigned char*)data, pSa.GetCount(), &pGuid, lOffset);
	if (hr == S_OK && pGuid != nullptr)
	{
		CComBSTR bstrGuid(pGuid);
		bstrGuid.CopyTo(lpwszGUID);
		delete[] pGuid;
	}
	SafeArrayUnaccessData(pSa);
	pSa.Detach();

	return hr;
}

STDMETHODIMP CScanner::LoadSignatures(BSTR lpwszPath)
{
	auto result = scandll::LoadSignatures(lpwszPath);

	return result;
}
