// MyFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MoonLighter_Tool.h"
#include "MyFormView.h"


// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW)
{

}

CMyFormView::~CMyFormView()
{
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_BN_CLICKED(IDC_MAPTOOL, &CMyFormView::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_EXTRACTPATH, &CMyFormView::OnBnClickedExtractPath)
END_MESSAGE_MAP()


// CMyFormView 진단입니다.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView 메시지 처리기입니다.


void CMyFormView::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMyFormView::OnBnClickedExtractPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_ExtractPath.GetSafeHwnd())
		m_ExtractPath.Create(IDD_EXTRACTPATH);

	m_ExtractPath.ShowWindow(SW_SHOW);
}
