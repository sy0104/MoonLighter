
// MoonLighter_ToolView.cpp : CMoonLighter_ToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MoonLighter_Tool.h"
#endif

#include "MoonLighter_ToolDoc.h"
#include "MoonLighter_ToolView.h"
#include "Graphic_Device.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWND;

// CMoonLighter_ToolView

IMPLEMENT_DYNCREATE(CMoonLighter_ToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMoonLighter_ToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMoonLighter_ToolView 생성/소멸

CMoonLighter_ToolView::CMoonLighter_ToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMoonLighter_ToolView::~CMoonLighter_ToolView()
{
	CGraphic_Device::Destroy_Instance();
}

BOOL CMoonLighter_ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CMoonLighter_ToolView 그리기

void CMoonLighter_ToolView::OnDraw(CDC* /*pDC*/)
{
	CMoonLighter_ToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphic_Device::Get_Instance()->Render_Begin();



	CGraphic_Device::Get_Instance()->Render_End();
}


// CMoonLighter_ToolView 인쇄

BOOL CMoonLighter_ToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMoonLighter_ToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMoonLighter_ToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMoonLighter_ToolView 진단

#ifdef _DEBUG
void CMoonLighter_ToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMoonLighter_ToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMoonLighter_ToolDoc* CMoonLighter_ToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMoonLighter_ToolDoc)));
	return (CMoonLighter_ToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMoonLighter_ToolView 메시지 처리기


void CMoonLighter_ToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	g_hWND = m_hWnd;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
	{
		ERR_MSG(L"ToolView - Graphic_Device Ready Failed");
		return;
	}

	
	// 스크롤 크기 조정
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILEY >> 1) * TILECY));

	// 창 크기 조정 - view의 크기만 바꿈
	// 스크린 좌표를 클라이언트 좌표로 변환
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());

	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int iWidth = rcMain.right - rcView.right;
	int iHeight = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, WINCX + iWidth, WINCY + iHeight, SWP_NOZORDER);
}
