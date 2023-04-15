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
	// ��� ���� �����ϴ� �Լ�
	static void DirInfoExtraction(const wstring& strFilePath, list<FILEINFO*>& listFileInfo);
	// ���� ���� ���� �Լ�
	static int DirFileCount(const wstring& strFilePath);
};


#endif