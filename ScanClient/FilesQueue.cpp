#include "stdafx.h"
#include "FilesQueue.h"


CFilesQueue::CFilesQueue()
{
}


CFilesQueue::~CFilesQueue()
{
}

void CFilesQueue::Push(const CString& sPath)
{
	m_pQueue.push(sPath);
}

const CString CFilesQueue::Pop()
{
	return m_pQueue.pop();
}

bool CFilesQueue::Empty()
{
	return (m_pQueue.size() == 0);
}
