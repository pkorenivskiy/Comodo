// ScanClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Logger/Logger.h"
#include "../scanlib/scanlib.h"
#include "DirReader.h"
#include "FilesScanner.h"
#include "ComFilesScanner.h"
#include "../include/MalwareDefs.h"

int GetProcCnt();

int wmain(int argc, wchar_t *argv[])
{
	auto pLogger = CreateConsoleLogger();

	if (argc < 3)
	{
		pLogger->Info(L"Usage : ");
		pLogger->Info(L"ScanClient <pathToSignaryFile> <pathToScan>");
		return 0;
	}

	auto pQueue = std::shared_ptr<CFilesQueue>(new CFilesQueue());
	auto pReader = CAutoPtr<CDirReader>(new CDirReader(pQueue));
	pReader->ReadDir(argv[1]);

	/*LoadSignatures(L"db.txt");
	CFilesScanner filesScanner(GetProcCnt(), pQueue, pLogger);*/

	CComFilesScanner filesScanner(GetProcCnt(), pQueue, pLogger, argv[2]);

	HANDLE hFake = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	while (pQueue->Empty() == false)
	{
		::WaitForSingleObject(hFake, 150);
		::ResetEvent(hFake);
	}
	::CloseHandle(hFake);

	pQueue->Stop();
	filesScanner.Stop();		

	_CrtDumpMemoryLeaks();

	return 0;
}

int GetProcCnt()
{
	//return 1;
	SYSTEM_INFO siSysInfo;

	GetSystemInfo(&siSysInfo);

	return siSysInfo.dwNumberOfProcessors;
}