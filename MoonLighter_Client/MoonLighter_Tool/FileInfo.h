#pragma once

#ifndef __FILEINFO_H__
#define __FILEINFO_H__

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString ConvertRelativePath(const CString& strFullPath);
	// 경로 정보 추출하는 함수
	static void DirInfoExtraction(const wstring& strFilePath, list<FILEINFO*>& listFileInfo);
	// 파일 개수 세는 함수
	static int DirFileCount(const wstring& strFilePath);
};


#endif