#include "stdafx.h"
#include "Potal.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "Player.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"


CPotal::CPotal()
	: m_ePreScene(END), m_eCurScene(END), m_wstrObjectKey(L""), m_wstrStateKey(L"")
{
}


CPotal::~CPotal()
{
	Release_Object();
}

HRESULT CPotal::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };


	m_ePreScene = IDLE;
	m_eCurScene = IDLE;
	m_wstrStateKey = L"IDLE";


	return S_OK;
}

int CPotal::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	Frame_Move();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CPotal::LateUpdate_Object()
{
	// 플레이어랑 충돌체크
	RECT rc = {};

	RECT rPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Rect();


	if (IntersectRect(&rc, &m_tRect, &CObjMgr::Get_Instance()->Get_Player()->Get_Rect()))
	{
		if (CKeyMgr::Get_Instance()->Key_Up('Z'))
		{

			if (m_wstrObjectKey == L"POTAL_A")
			{
				if (m_tInfo.vPos.y >= 600.f)			// Golem King
					CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::STAGE1);
				else									// Tech Guardian
					CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::STAGE3);
				return;
			}
			else if (m_wstrObjectKey == L"POTAL_B")		// Plant
			{
				CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::STAGE2);
				return;
			}
		}
	}

}

void CPotal::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 1.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 1.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 1.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 1.5f));


//#ifdef _DEBUG
//	D3DXVECTOR2 vLine[5] =
//	{
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y }
//	};
//
//	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
//	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
//	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
//
//#endif

}

void CPotal::Release_Object()
{
}

void CPotal::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;


	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
	{

		m_tFrame.dFrameStart = 0.0;
	}

}

void CPotal::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		//// Idle ////
		case CPotal::IDLE:
			if (m_wstrObjectKey == L"POTAL_A")
			{
				m_wstrStateKey = L"IDLE";
				m_tFrame = { 0.0, 6.0 };
			}
			else if (m_wstrObjectKey == L"POTAL_B")
			{
				m_wstrStateKey = L"IDLE";
				m_tFrame = { 0.0, 6.0 };
			}
			else
			{
				m_wstrObjectKey = L"POTAL_C";
				m_wstrStateKey = L"IDLE";
				m_tFrame = { 0.0, 7.0 };
			}
			break;

		//// Open ////
		case CPotal::OPEN:
			if (m_wstrObjectKey == L"POTAL_A")
			{
				m_wstrStateKey = L"OPEN";
				m_tFrame = { 0.0, 6.0 };
			}
			else if (m_wstrObjectKey == L"POTAL_B")
			{
				m_wstrStateKey = L"OPEN";
				m_tFrame = { 0.0, 7.0 };
			}
			else
			{
				m_wstrObjectKey = L"POTAL_C";
				m_wstrStateKey = L"OPEN";
				m_tFrame = { 0.0, 30.0 };
			}
			break;

		//// Close ////
		case CPotal::CLOSE:
			if (m_wstrObjectKey == L"POTAL_A")
			{
				m_wstrStateKey = L"CLOSE";
				m_tFrame = { 0.0, 9.0 };
			}
			else if (m_wstrObjectKey == L"POTAL_B")
			{
				m_wstrStateKey = L"CLOSE";
				m_tFrame = { 0.0, 7.0 };
			}
			else
			{
				m_wstrObjectKey = L"POTAL_C";
				m_wstrStateKey = L"CLOSE";
				m_tFrame = { 0.0, 35.0 };
			}
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
