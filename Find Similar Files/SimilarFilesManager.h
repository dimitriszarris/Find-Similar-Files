#pragma once
#include <vector>
#include <map>
#include <string>
#include <thread>

namespace SimilarFiles
{
	typedef struct
	{
		DWORD			nFileSize;
		std::wstring	wPath;
	}FILE_PATH_ATTRIBUTES;

	typedef std::map<std::wstring, std::vector<FILE_PATH_ATTRIBUTES>> MapFileAttributes;
	typedef std::vector<std::wstring> VectorFiles;

#define to_Megabytes(n) n/(1024*1024)
#define to_Kilobytes(n) n/1024

	class CSimilarFilesManager
	{

		MapFileAttributes _mapFilesAttributes;
		VectorFiles _vDuplicates;
		unsigned __int64 _nDiskSpaceOccupied;

		std::thread _thread;

	public:

		std::vector<std::wstring> SearchDuplicateFiles(std::wstring folder);
		void Clear();

		MapFileAttributes GetMapFilesAttributes() const { return _mapFilesAttributes; }
		VectorFiles GetVDuplicates() const { return _vDuplicates; }

		void GetFileInfo(std::wstring strFileSize, std::wstring &filename, std::wstring &filesize);
		VectorFiles GetFilePaths(std::wstring strFileSize);
		unsigned __int64 GetDiskSpaceOccupied() { return to_Megabytes(_nDiskSpaceOccupied); }
	};
}
