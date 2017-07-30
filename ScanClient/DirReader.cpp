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
	// with modern c++
	//for (std::experimental::filesystem::v1::recursive_directory_iterator next(
	//	std::experimental::filesystem::v1::path(sPath.GetString())), end; next != end; ++next)
	//{
	//	m_pQueue->Push(next->path().c_str());
	//}

	// with win32 and recursive
	CString sRoot(sPath);
	if (sRoot[sRoot.GetLength() - 1] != L'\\')
		sRoot.Append(L"\\");	

	WIN32_FIND_DATA ffd;
	auto hh = FindFirstFile(sRoot + L"*.*", &ffd);

	if (hh != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
					continue;
				ReadDir(sRoot + ffd.cFileName);
			}
			else
			{
				m_pQueue->Push(sRoot + ffd.cFileName);
			}
		} while (FindNextFile(hh, &ffd) != 0);
	}
}