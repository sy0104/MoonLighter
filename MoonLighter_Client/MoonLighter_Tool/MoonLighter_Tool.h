
// MoonLighter_Tool.h : MoonLighter_Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMoonLighter_ToolApp:
// �� Ŭ������ ������ ���ؼ��� MoonLighter_Tool.cpp�� �����Ͻʽÿ�.
//

class CMoonLighter_ToolApp : public CWinAppEx
{
public:
	CMoonLighter_ToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMoonLighter_ToolApp theApp;
