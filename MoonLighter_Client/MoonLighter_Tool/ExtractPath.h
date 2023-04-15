#pragma once
#include "afxwin.h"


// CExtractPath ��ȭ �����Դϴ�.

class CExtractPath : public CDialog
{
	DECLARE_DYNAMIC(CExtractPath)

public:
	CExtractPath(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CExtractPath();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTRACTPATH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	list<FILEINFO*>		m_ListFileInfo;
	CListBox			m_PathList;

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();

	afx_msg void OnDropFiles(HDROP hDropInfo);

	void HorizontalScroll();
};
