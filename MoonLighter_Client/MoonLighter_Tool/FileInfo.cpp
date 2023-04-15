#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString& strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurrentDirectory[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurrentDirectory);
	PathRelativePathTo(szRelativePath, szCurrentDirectory, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & strFilePath, list<FILEINFO*>& listFileInfo)
{
	CFileFind find;

	wstring wstrFullPath = strFilePath + L"\\*.*";
	BOOL bContinue = find.FindFile(wstrFullPath.c_str());

	while (bContinue)
	{
		bContinue = find.FindNextFile();

		if (find.IsDots())
			continue;

		else if (find.IsDirectory())
			DirInfoExtraction(find.GetFilePath().GetString(), listFileInfo);

		else
		{
			if (find.IsSystem())
				continue;

			FILEINFO*	pFileInfo = new FILEINFO;
			TCHAR		szPathBuff[MAX_PATH] = L"";

			lstrcpy(szPathBuff, find.GetFilePath());
			PathRemoveFileSpec(szPathBuff);
			pFileInfo->iCount = CFileInfo::DirFileCount(szPathBuff);


			wstring wstrTextureName = find.GetFileTitle().GetString();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d.png";

			TCHAR szFilePath[MAX_PATH] = L"";

			PathCombine(szFilePath, szPathBuff, wstrTextureName.c_str());
			pFileInfo->wstrFilePath = CFileInfo::ConvertRelativePath(szFilePath);

			PathRemoveFileSpec(szFilePath);
			pFileInfo->wstrStateKey = PathFindFileName(szFilePath);

			PathRemoveFileSpec(szFilePath);
			pFileInfo->wstrObjectKey = PathFindFileName(szFilePath);

			listFileInfo.emplace_back(pFileInfo);
			bContinue = FALSE;
		}
	}
}

int CFileInfo::DirFileCount(const wstring & strFilePath)
{
	CFileFind	find;
	wstring wstrFullPath = strFilePath + L"\\*.*";

	bool bContinue = find.FindFile(wstrFullPath.c_str());

	int iCount = 0;

	while (bContinue)
	{
		bContinue = find.FindNextFile();
		if (find.IsDots() || find.IsSystem())
			continue;
		++iCount;
	}

	return iCount;
}
