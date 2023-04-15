#include "stdafx.h"
#include "MainApp.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "FrameMgr.h"
#include "Graphic_Device.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"


CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
	Release_MainApp();
}

void CMainApp::Ready_MainApp()
{
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Create Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->ReadImagePath(L"../Data/PathInfo.txt")))
	{
		ERR_MSG(L"Read Image Path Failed");
		return;
	}

	if (FAILED(CTimeMgr::Get_Instance()->Ready_TimeMgr()))
	{
		ERR_MSG(L"TimeMgr Ready Failed");
		return;
	}

	CSoundMgr::Get_Instance()->Initialize();
	CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::LOGO);
}

void CMainApp::Update_MainApp()
{
	CSceneMgr::Get_Instance()->Update_SceneMgr();

}

void CMainApp::LateUpdate_MainApp()
{
	CSceneMgr::Get_Instance()->LateUpdate_SceneMgr();	
	CKeyMgr::Get_Instance()->Key_Update();
	CTimeMgr::Get_Instance()->Update_TimeMgr();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainApp::Render_MainApp(const CFrameMgr& rFrameMgr)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	CSceneMgr::Get_Instance()->Render_SceneMgr();
	const_cast<CFrameMgr&>(rFrameMgr).Render_FrameMgr();
	CGraphic_Device::Get_Instance()->Render_End();
}

void CMainApp::Release_MainApp()
{
	CTextureMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();

	CObjMgr::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
}
