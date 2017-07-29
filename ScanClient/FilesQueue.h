#pragma once

#include "FilesQueue.h"
#include "../include/TSQueue.h"

class CFilesQueue
{
public:
	CFilesQueue();
	~CFilesQueue();

public:
	void Push(const CString& sPath);
	const CString Pop();
	bool Empty();
	void Stop() { m_pQueue.stop(); };

private:
	TSQueue<CString> m_pQueue;
};

