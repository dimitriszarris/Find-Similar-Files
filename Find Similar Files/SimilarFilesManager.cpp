#include "stdafx.h"
#include "SimilarFilesManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace SimilarFiles;

vector<wstring> CSimilarFilesManager::SearchDuplicateFiles(wstring folder)
{
	vector<wstring> vfiles{}, vDuplicates{};

	wstring wSearchFolder = folder + L"\\*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(wSearchFolder.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			wstring delimiter = L":";
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//__raise show_current_folder_event(folder);
				map<wstring, vector<FILE_PATH_ATTRIBUTES>>::iterator iter;
				if ((iter = _mapFilesAttributes.find(fd.cFileName + delimiter + to_wstring(fd.nFileSizeLow))) != _mapFilesAttributes.end())
				{
					FILE_PATH_ATTRIBUTES fpa;
					fpa.nFileSize = fd.nFileSizeLow;
					_nDiskSpaceOccupied += fd.nFileSizeLow;
					fpa.wPath = folder;
					_vDuplicates.push_back(fd.cFileName + delimiter + to_wstring(fd.nFileSizeLow));
					iter = _mapFilesAttributes.find(fd.cFileName + delimiter + to_wstring(fd.nFileSizeLow));
					iter->second.push_back(fpa);
				}
				else
				{
					vector<FILE_PATH_ATTRIBUTES> vfpa;
					FILE_PATH_ATTRIBUTES fpa;
					fpa.nFileSize = fd.nFileSizeLow;
					fpa.wPath = folder;
					vfpa.push_back(fpa);
					_mapFilesAttributes.insert(pair<wstring, vector<FILE_PATH_ATTRIBUTES>>(fd.cFileName + delimiter + to_wstring(fd.nFileSizeLow), vfpa));
				}
			}
			else if (fd.cFileName[0] != '.')
			{
				wstring wPath = folder + L"\\" + fd.cFileName;
				SearchDuplicateFiles(wPath);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return _vDuplicates;
}

void CSimilarFilesManager::Clear()
{
	_nDiskSpaceOccupied = 0;
	_vDuplicates.clear();
	_mapFilesAttributes.clear();
}

void CSimilarFilesManager::GetFileInfo(wstring wFileSize, wstring &filename, wstring &filesize)
{
	MapFileAttributes::iterator iter;
	iter = _mapFilesAttributes.find(wFileSize);
	filename = iter->first.substr(0, iter->first.find(':')).c_str();
	wstring wtmp = iter->first.substr(iter->first.find(':') + 1, iter->first.length() - 1);
	unsigned __int64 ntmp;
	ntmp = stoull(wtmp);
	filesize = to_wstring(ntmp);
}

VectorFiles CSimilarFilesManager::GetFilePaths(wstring wFileSize)
{
	VectorFiles filepaths;
	MapFileAttributes::iterator iter;
	iter = _mapFilesAttributes.find(wFileSize);
	if (iter == _mapFilesAttributes.end())
		return filepaths;

	for (auto w : iter->second)
		filepaths.push_back(w.wPath);

	return filepaths;
}
