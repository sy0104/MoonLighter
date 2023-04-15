
// MoonLighter_ToolView.cpp : CMoonLighter_ToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMoonLighter_ToolView ����/�Ҹ�

CMoonLighter_ToolView::CMoonLighter_ToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMoonLighter_ToolView::~CMoonLighter_ToolView()
{
	CGraphic_Device::Destroy_Instance();
}

BOOL CMoonLighter_ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CMoonLighter_ToolView �׸���

void CMoonLighter_ToolView::OnDraw(CDC* /*pDC*/)
{
	CMoonLighter_ToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphic_Device::Get_Instance()->Render_Begin();



	CGraphic_Device::Get_Instance()->Render_End();
}


// CMoonLighter_ToolView �μ�

BOOL CMoonLighter_ToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMoonLighter_ToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMoonLighter_ToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMoonLighter_ToolView ����

#ifdef _DEBUG
void CMoonLighter_ToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMoonLighter_ToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMoonLighter_ToolDoc* CMoonLighter_ToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMoonLighter_ToolDoc)));
	return (CMoonLighter_ToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMoonLighter_ToolView �޽��� ó����


void CMoonLighter_ToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	g_hWND = m_hWnd;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
	{
		ERR_MSG(L"ToolView - Graphic_Device Ready Failed");
		return;
	}

	
	// ��ũ�� ũ�� ����
	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILEY >> 1) * TILECY));

	// â ũ�� ���� - view�� ũ�⸸ �ٲ�
	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ
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
