// ExtractPath.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MoonLighter_Tool.h"
#include "ExtractPath.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CExtractPath 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExtractPath, CDialog)

CExtractPath::CExtractPath(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EXTRACTPATH, pParent)
{

}

CExtractPath::~CExtractPath()
{
	for (auto& pFileInfo : m_ListFileInfo)
		Safe_Delete(pFileInfo);
}

void CExtractPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATHLIST, m_PathList);
}


BEGIN_MESSAGE_MAP(CExtractPath, CDialog)
	ON_BN_CLICKED(IDC_SAVE, &CExtractPath::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CExtractPath::OnBnClickedLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CExtractPath 메시지 처리기입니다.


void CExtractPath::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	wofstream	fOut;

	fOut.open("../Data/PathInfo.txt");

	if (!fOut.fail())
	{
		for (auto& pFileInfo : m_ListFileInfo)
			fOut << pFileInfo->wstrFilePath << L"|" << pFileInfo->wstrObjectKey << L"|" << pFileInfo->wstrStateKey << L"|" << pFileInfo->iCount << endl;

	}

	fOut.close();
	WinExec("notepad.exe ../Data/PathInfo.txt", SW_SHOW);
}


void CExtractPath::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wifstream	fIn;
	fIn.open("../Data/PathInfo.txt");

	UpdateData(TRUE);

	if (!fIn.fail())
	{
		TCHAR	szFilePath[MAX_PATH] = L"";
		TCHAR	szObjectKey[MAX_PATH] = L"";
		TCHAR	szStateKey[MAX_PATH] = L"";
		TCHAR	szCount[MAX_PATH] = L"";

		m_PathList.ResetContent();

		for (auto& pFileInfo : m_ListFileInfo)
			Safe_Delete(pFileInfo);
		m_ListFileInfo.clear();

		while (true)
		{
			fIn.getline(szFilePath, MAX_PATH, L'|');
			fIn.getline(szObjectKey, MAX_PATH, L'|');
			fIn.getline(szStateKey, MAX_PATH, L'|');
			fIn.getline(szCount, MAX_PATH);

			// eof(): 파일 끝
			if (fIn.eof())
				break;

			FILEINFO*	pFileInfo = new FILEINFO;
			pFileInfo->wstrFilePath = szFilePath;
			pFileInfo->wstrObjectKey = szObjectKey;
			pFileInfo->wstrStateKey = szStateKey;
			pFileInfo->iCount = _ttoi(szCount);

			wstring wstrCombine = L"";
			wstrCombine = pFileInfo->wstrFilePath + L"|" + pFileInfo->wstrObjectKey + L"|" + pFileInfo->wstrStateKey + L"|" + szCount;
			m_ListFileInfo.emplace_back(pFileInfo);
			m_PathList.AddString(wstrCombine.c_str());
		}
	}

	fIn.close();
	HorizontalScroll();
	UpdateData(FALSE);

}

void CExtractPath::HorizontalScroll()
{
	CString		strName;
	CSize		tSize;
	int			iCX = 0;

	CDC* pDC = m_PathList.GetDC();


	for (int i = 0; i < m_PathList.GetCount(); ++i)
	{
		m_PathList.GetText(i, strName);
		tSize = pDC->GetTextExtent(strName);

		if (tSize.cx > iCX)
			iCX = tSize.cx;
	}

	m_PathList.ReleaseDC(pDC);

	if (iCX > m_PathList.GetHorizontalExtent())
		m_PathList.SetHorizontalExtent(iCX);

}


void CExtractPath::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFullFilePath[MAX_PATH] = L"";
	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFullFilePath, m_ListFileInfo);
	}


	m_PathList.ResetContent();

	wstring		wstrCombine;
	TCHAR		szBuff[32] = L"";

	for (auto& pFileInfo : m_ListFileInfo)
	{
		// 숫자를 문자열로 바꿀겨
		// _itow_s(count, 문자열, 진수)
		_itow_s(pFileInfo->iCount, szBuff, 10);
		wstrCombine = pFileInfo->wstrFilePath + L"|" + pFileInfo->wstrObjectKey + L"|" + pFileInfo->wstrStateKey + L"|" + szBuff;

		m_PathList.AddString(wstrCombine.c_str());
	}

	HorizontalScroll();
	UpdateData(FALSE);
}
