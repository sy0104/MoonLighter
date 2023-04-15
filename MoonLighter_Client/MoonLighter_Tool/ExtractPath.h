#pragma once
#include "afxwin.h"


// CExtractPath 대화 상자입니다.

class CExtractPath : public CDialog
{
	DECLARE_DYNAMIC(CExtractPath)

public:
	CExtractPath(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExtractPath();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTRACTPATH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	list<FILEINFO*>		m_ListFileInfo;
	CListBox			m_PathList;

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

	afx_msg void OnDropFiles(HDROP hDropInfo);

	void HorizontalScroll();
};
