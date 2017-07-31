#pragma once

#include <vector>
#include <thread>

#include "FilesQueue.h"
#include "../Logger/Logger.h"

class CFilesScanner
{
public:
	CFilesScanner(int nThreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger);
	~CFilesScanner();

protected:
	CFilesScanner() {}

public:
	void Stop();

	void threadFunc(int i);	

protected:
	CLogger* m_pLogger;
	std::shared_ptr<CFilesQueue> m_pQueue;
	std::vector<std::thread> m_vThreads;
	bool m_bStop;
};

