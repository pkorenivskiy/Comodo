#include "stdafx.h"
#include "DirReader.h"

#include <filesystem>


CDirReader::CDirReader(std::shared_ptr<CFilesQueue> pQueue)
	
{
	m_pQueue = pQueue;
}


CDirReader::~CDirReader()
{
}

void CDirReader::ReadDir(const CString& sPath)
{
	for (std::experimental::filesystem::v1::recursive_directory_iterator next(
		std::experimental::filesystem::v1::path(sPath.GetString())), end; next != end; ++next)
	{
		m_pQueue->Push(next->path().c_str());
	}
}