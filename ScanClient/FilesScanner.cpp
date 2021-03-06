#include "stdafx.h"
#include "FilesScanner.h"
#include "../scanlib/scanlib.h"
#include "../include/MalwareDefs.h"


CFilesScanner::CFilesScanner(int nThreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger)
	:m_pQueue(pQueue)
	,m_pLogger(pLogger)
	,m_bStop(false)
{
	for (auto i = 0; i < nThreads; ++i)
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
			
			wchar_t* pGuid;
			long nOffset = 0;

			auto res = scandll::ScanFile(f, &pGuid, &nOffset);

			if (res == S_OK)
			{
				if (pGuid == nullptr)
				{
					msg.Append(L"ok");
					m_pLogger->Info(msg);
				}
				else
				{
					msg.Append(L"infected\n");
					msg.AppendFormat(L"offset: %d, malware : %s", nOffset, pGuid);
					m_pLogger->Error(msg);
					delete[] pGuid;
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
