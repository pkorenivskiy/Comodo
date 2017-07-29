// ScanClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Logger/Logger.h"
#include "../scanlib/scanlib.h"
#include "DirReader.h"
#include "FilesScanner.h"
#include "../include/MalwareDefs.h"

int GetProcCnt();

int main()
{
	auto pLogger = CreateConsoleLogger();

	CString str("Debug");
	pLogger->Debug(str);
	str = "Information";
	pLogger->Info(str);
	str = "Warning";
	pLogger->Warn(str);
	str = "Error";
	pLogger->Error(str);

	TScanResult* pScanResult;
	size_t count = 0;
	HRESULT res = ScanFile(L"C:\\MyProjects\\DELETE\\Comodo\\debug\\virus.dll", &pScanResult, count);
	res = ScanFile(L"C:\\MyProjects\\DELETE\\Comodo\\debug\\1sec.bin", &pScanResult, count);

	auto pQueue = std::shared_ptr<CFilesQueue>(new CFilesQueue());
	auto pReader = CAutoPtr<CDirReader>(new CDirReader(pQueue));
	pReader->ReadDir(L"C:\\MyProjects\\DELETE\\Comodo\\");

	CFilesScanner filesScanner(GetProcCnt(), pQueue, pLogger);

	HANDLE hFake = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	while (pQueue->Empty() == false)
	{
		::WaitForSingleObject(hFake, 150);
		::ResetEvent(hFake);
	}
	::CloseHandle(hFake);

	filesScanner.Stop();
	pQueue->Stop();

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