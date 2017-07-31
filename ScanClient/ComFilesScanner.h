#pragma once

#include "FilesScanner.h"

//#import "c:/myprojects/delete/comodo/debug/ScanSrv.Dll"

#include "scansrv.tlh"

using namespace ScanSrvLib;

class CComFilesScanner
{
public:
	CComFilesScanner(int nThreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger, const wchar_t* lpwszPathSign);
	~CComFilesScanner();

public:
	void Stop();

private:
	void threadFunc(int i);

private:
	CComPtr<IScanner> m_pScanner;
	CLogger* m_pLogger;
	std::shared_ptr<CFilesQueue> m_pQueue;
	std::vector<std::thread> m_vThreads;
	bool m_bStop;
};

