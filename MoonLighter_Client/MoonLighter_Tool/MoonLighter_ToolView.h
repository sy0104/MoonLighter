
// MoonLighter_ToolView.h : CMoonLighter_ToolView Ŭ������ �������̽�
//

#pragma once

class CMoonLighter_ToolDoc;

class CMoonLighter_ToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMoonLighter_ToolView();
	DECLARE_DYNCREATE(CMoonLighter_ToolView)

// Ư���Դϴ�.
public:
	CMoonLighter_ToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMoonLighter_ToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // MoonLighter_ToolView.cpp�� ����� ����
inline CMoonLighter_ToolDoc* CMoonLighter_ToolView::GetDocument() const
   { return reinterpret_cast<CMoonLighter_ToolDoc*>(m_pDocument); }
#endif

