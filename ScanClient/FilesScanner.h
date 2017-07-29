#pragma once

#include <vector>
#include <thread>

#include "FilesQueue.h"
#include "../Logger/Logger.h"

class CFilesScanner
{
public:
	CFilesScanner(int nTreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger);
	~CFilesScanner();

public:
	void Stop();

private:
	void threadFunc(int i);	

private:
	CLogger* m_pLogger;
	std::shared_ptr<CFilesQueue> m_pQueue;
	std::vector<std::thread> m_vThreads;
	bool m_bStop;
};

