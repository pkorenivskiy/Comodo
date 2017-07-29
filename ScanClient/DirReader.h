#pragma once

#include <memory>
#include "FilesQueue.h"

class CDirReader
{
public:
	CDirReader(std::shared_ptr<CFilesQueue> pQueue);
	~CDirReader();

public:
	void ReadDir(const CString& sPath);

private:
	std::shared_ptr<CFilesQueue> m_pQueue;
};

