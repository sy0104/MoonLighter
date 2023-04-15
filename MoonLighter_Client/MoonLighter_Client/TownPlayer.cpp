#include "stdafx.h"
#include "TownPlayer.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "Inven.h"
#include "Npc.h"

#include "Item.h"
#include "Showcase.h"

CTownPlayer::CTownPlayer()
	: m_ePreScene(STATE_END), m_eCurScene(STATE_END), m_ePlayerDir(DIR::END), m_wstrObjectKey(L""), m_wstrStateKey(L"")
	, m_pInven(nullptr), m_bCreateInven(false), m_bCreateNpc(false), m_dNpcTime(0.0), m_bOpenShop(false), m_bStop(false), m_bStopTime(0.0), m_bShowcase(false)
{
}


CTownPlayer::~CTownPlayer()
{
	Release_Object();
}

HRESULT CTownPlayer::Ready_Object()
{
	m_tInfo.vPos = { 140.f, 30.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 4.f;

	m_ePreScene = IDLE_DOWN;
	m_eCurScene = IDLE_DOWN;
	m_wstrObjectKey = L"Player_Shop_Idle";
	m_wstrStateKey = L"Down";
	m_tFrame = { 0.0, 10.0 };

	// Inven
	if (CObjMgr::Get_Instance()->Get_List(OBJ::INVEN).empty())
	{
		m_pInven = CAbstractFactory<CInven>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ::INVEN, m_pInven);
	}
	else
	{
		m_pInven = CObjMgr::Get_Instance()->Get_List(OBJ::INVEN).front();
	}


	return S_OK;
}

int CTownPlayer::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bOpenShop)
		Open_Shop();

	if (m_bStop)
		m_fSpeed = 0.f;
	else
		m_fSpeed = 4.f;


	Key_Check();
	Frame_Move();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CTownPlayer::LateUpdate_Object()
{
}

void CTownPlayer::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));



#ifdef _DEBUG

	// 플레이어 좌표 출력
	D3DXMATRIX	matFontTrans;
	D3DXMatrixTranslation(&matFontTrans, 100.f, 50.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	swprintf_s(m_szPos, L"X: %f   Y: %f", m_tInfo.vPos.x, m_tInfo.vPos.y);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPos, lstrlen(m_szPos), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	// 충돌박스 출력
	D3DXVECTOR2 vLine[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

#endif

}

void CTownPlayer::Release_Object()
{
	//for (auto& pShowcase : m_vecShowcase)
	//	Safe_Delete(pShowcase);
	//m_vecShowcase.clear();
}

void CTownPlayer::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_tFrame.dFrameStart = 0.0;
	}
}

void CTownPlayer::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		//// IDLE ////
		case IDLE_LEFT:
			m_wstrObjectKey = L"Player_Shop_Idle";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 10.0 };
			break;
		case IDLE_RIGHT:
			m_wstrObjectKey = L"Player_Shop_Idle";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 10.0 };
			break;
		case IDLE_UP:
			m_wstrObjectKey = L"Player_Shop_Idle";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 10.0 };
			break;
		case IDLE_DOWN:
			m_wstrObjectKey = L"Player_Shop_Idle";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 10.0 };
			break;
		//// WALK ////
		case WALK_LEFT:
			m_wstrObjectKey = L"Player_Shop_Walk";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_RIGHT:
			m_wstrObjectKey = L"Player_Shop_Walk";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_UP:
			m_wstrObjectKey = L"Player_Shop_Walk";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_DOWN:
			m_wstrObjectKey = L"Player_Shop_Walk";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 8.0 };
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CTownPlayer::Key_Check()
{
	// Walk
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_ePlayerDir = DIR::LEFT;
		m_eCurScene = WALK_LEFT;
		m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_ePlayerDir = DIR::RIGHT;
		m_eCurScene = WALK_RIGHT;
		m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_ePlayerDir = DIR::UP;
		m_eCurScene = WALK_UP;
		m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_ePlayerDir = DIR::DOWN;
		m_eCurScene = WALK_DOWN;
		m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
	}
	// -----------------------------------------------------------------------------------
	// Idle
	else
	{
		if (m_ePlayerDir == DIR::LEFT)
			m_eCurScene = IDLE_LEFT;
		else if (m_ePlayerDir == DIR::RIGHT)
			m_eCurScene = IDLE_RIGHT;
		else if (m_ePlayerDir == DIR::UP)
			m_eCurScene = IDLE_UP;
		else
			m_eCurScene = IDLE_DOWN;
	}
	// -----------------------------------------------------------------------------------
	// Inven
	if (CKeyMgr::Get_Instance()->Key_Down('I'))
	{
		if (!m_bCreateInven)
		{
			dynamic_cast<CInven*>(m_pInven)->Set_Render();
			m_bCreateInven = true;
		}

		else if (m_bCreateInven)
		{
			dynamic_cast<CInven*>(m_pInven)->Set_Hide();
			m_bCreateInven = false;

		}
	}
	// -----------------------------------------------------------------------------------
	// OpenShop
	if (CKeyMgr::Get_Instance()->Key_Down('O'))
		m_bOpenShop = true;

	// -----------------------------------------------------------------------------------
	// Stop
	if (m_bStop)
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)
			|| CKeyMgr::Get_Instance()->Key_Up(VK_UP) || CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
			m_bStop = false;
	}
	// -----------------------------------------------------------------------------------
	// Sell
	//if (CKeyMgr::Get_Instance()->Key_Down('A'))
	//{
	//	if (!m_bShowcase)
	//	{
	//		for (auto& iter = m_vecShowcase.begin(); iter != m_vecShowcase.end(); ++iter)
	//		{
	//			dynamic_cast<CShowcase*>(*iter)->Set_Render();
	//		}
	//	
	//		m_bShowcase = true;
	//	}

	//	else if (m_bShowcase)
	//	{
	//		for (auto& iter = m_vecShowcase.begin(); iter != m_vecShowcase.end(); ++iter)
	//		{
	//			dynamic_cast<CShowcase*>(*iter)->Set_Hide();
	//		}

	//		m_bShowcase = false;
	//	}

	//}
	// -----------------------------------------------------------------------------------

}

void CTownPlayer::Open_Shop()
{
	m_dNpcTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (!m_bCreateNpc)
	{
		// Nathan
		if (m_dNpcTime >= 0.0 && m_dNpcTime < 1.0)
		{
			CObj* pObj = CAbstractFactory<CNpc>::Create(670.f, 510.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, pObj);
			dynamic_cast<CNpc*>(pObj)->Set_ObjectKey(L"Nathan");
			dynamic_cast<CNpc*>(pObj)->Set_StateKey(L"WALK_UP");
			dynamic_cast<CNpc*>(pObj)->Set_Frame(8.0);
			m_bCreateNpc = true;
		}

		// Sakura
		else if (m_dNpcTime >= 3.0 && m_dNpcTime < 4.0)
		{
			CObj* pObj = CAbstractFactory<CNpc>::Create(670.f, 510.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, pObj);
			dynamic_cast<CNpc*>(pObj)->Set_ObjectKey(L"Sakura");
			dynamic_cast<CNpc*>(pObj)->Set_StateKey(L"WALK_UP");
			dynamic_cast<CNpc*>(pObj)->Set_Frame(8.0);
			m_bCreateNpc = true;
		}

		// Worker
		else if (m_dNpcTime >= 5.0 && m_dNpcTime < 6.0)
		{
			CObj* pObj = CAbstractFactory<CNpc>::Create(670.f, 510.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, pObj);
			dynamic_cast<CNpc*>(pObj)->Set_ObjectKey(L"Worker");
			dynamic_cast<CNpc*>(pObj)->Set_StateKey(L"WALK_UP");
			dynamic_cast<CNpc*>(pObj)->Set_Frame(8.0);
			m_bCreateNpc = true;
		}

		// Rise
		else if (m_dNpcTime >= 7.0 && m_dNpcTime < 8.0)
		{
			CObj* pObj = CAbstractFactory<CNpc>::Create(670.f, 510.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, pObj);
			dynamic_cast<CNpc*>(pObj)->Set_ObjectKey(L"Rise");
			dynamic_cast<CNpc*>(pObj)->Set_StateKey(L"WALK_UP");
			dynamic_cast<CNpc*>(pObj)->Set_Frame(8.0);
			m_bCreateNpc = true;
		}
	}


	if (m_dNpcTime >= 1.0 && m_dNpcTime < 3.0)
		m_bCreateNpc = false;
	else if (m_dNpcTime >= 4.0 && m_dNpcTime < 5.0)
		m_bCreateNpc = false;
	else if (m_dNpcTime >= 6.0 && m_dNpcTime < 7.0)
		m_bCreateNpc = false;
}
