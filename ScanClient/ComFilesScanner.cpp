#include "stdafx.h"
#include <atlsafe.h>
#include <atlcomcli.h>

#include "ComFilesScanner.h"


CComFilesScanner::CComFilesScanner(int nThreads, std::shared_ptr<CFilesQueue> pQueue, CLogger* pLogger, const wchar_t* lpwszPathSign)
	:m_pQueue(pQueue)
	, m_pLogger(pLogger)
	, m_bStop(false)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	auto hr = m_pScanner.CoCreateInstance(__uuidof(Scanner));

	if (hr != S_OK)
		m_pScanner = nullptr;

	hr = m_pScanner->LoadSignatures(lpwszPathSign);

	for (auto i = 0; i < nThreads; ++i)
	{
		m_vThreads.push_back(std::thread(&CComFilesScanner::threadFunc, this, i));
	}
}

CComFilesScanner::~CComFilesScanner()
{
	CoUninitialize();
}

void CComFilesScanner::Stop()
{
	m_bStop = true;

	for (auto it = m_vThreads.begin(); it != m_vThreads.end(); it++)
	{
		if (it->joinable())
			it->join();
	}
}

void CComFilesScanner::threadFunc(int i)
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

			auto res = m_pScanner->ScanFile(f.GetString(), &nOffset, &pGuid);

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

