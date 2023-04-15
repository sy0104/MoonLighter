#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MainMap.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Town.h"
#include "Shop.h"
#include "PreStage.h"



IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_ePreScene(END), m_eCurScene(END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release_SceneMgr();
}


void CSceneMgr::Update_SceneMgr()
{
	m_pScene->Update_Scene();
}

void CSceneMgr::LateUpdate_SceneMgr()
{
	m_pScene->LateUpdate_Scene();
}

void CSceneMgr::Render_SceneMgr()
{
	m_pScene->Render_Scene();
}

void CSceneMgr::Release_SceneMgr()
{
	Safe_Delete(m_pScene);
}

bool CSceneMgr::Get_Scroll()
{
	// LOGO, TOWN, SHOP, MAP, PRESTAGE, STAGE1, STAGE2, STAGE3, END

	if (m_eCurScene == PRESTAGE)
		return false;

	return true;
}

HRESULT CSceneMgr::Change_Scene(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		SAFE_DELETE(m_pScene);

		switch (m_eCurScene)
		{
		case LOGO:
			m_pScene = new CLogo;
			break;
		case TOWN:
			m_pScene = new CTown;
			break;
		case SHOP:
			m_pScene = new CShop;
			break;
		case MAP:
			m_pScene = new CMainMap;
			break;
		case PRESTAGE:
			m_pScene = new CPreStage;
			break;
		case STAGE1:
			m_pScene = new CStage1;
			break;
		case STAGE2:
			m_pScene = new CStage2;
			break;
		case STAGE3:
			m_pScene = new CStage3;
			break;
		}
		m_pScene->Ready_Scene();
		m_ePreScene = m_eCurScene;
	}
	return S_OK;
}