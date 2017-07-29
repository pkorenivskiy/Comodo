#include "stdafx.h"
#include "FilesScanner.h"
#include "../scanlib/scanlib.h"
#include "../include/MalwareDefs.h"


CFilesScanner::CFilesScanner(int nTreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger)
	:m_pQueue(pQueue)
	,m_pLogger(pLogger)
	,m_bStop(false)
{
	for (auto i = 0; i < nTreads; ++i)
	{
		m_vThreads.push_back(std::thread(&CFilesScanner::threadFunc, this, i));
	}
}


CFilesScanner::~CFilesScanner()
{
	
}

void CFilesScanner::Stop()
{
	m_bStop = true;

	for (auto it = m_vThreads.begin(); it != m_vThreads.end(); it++)
	{
		if (it->joinable())
			it->join();
	}
}

void CFilesScanner::threadFunc(int i)
{
	while (!m_bStop)
	{
		const auto f = m_pQueue->Pop();
		if (f.IsEmpty() == false)
		{
			CString msg;
			msg.Format(L"[thread %d]: ", i);
			msg.Append(L"scan file ");
			msg.Append(f);
			msg.Append(L"...");
			
			TScanResult* pSanResult;
			size_t count = 0;

			auto res = ScanFile(f, &pSanResult, count);

			if (res == S_OK)
			{
				if (count == 0)
				{
					msg.Append(L"ok");
					m_pLogger->Info(msg);
				}
				else
				{
					msg.Append(L"infected\n");
					for (int i = 0; i < count; ++i)
					{
						msg.AppendFormat(L"offset: %d, malware : %s", pSanResult[i].Offset, pSanResult[i].Malware.GUID);
					}
					m_pLogger->Error(msg);
					delete[] pSanResult;
				}
			}
			else
			{
				msg.Append(L"Scanner error!");
				m_pLogger->Error(msg);
			}
		}
	}
}