#include "stdafx.h"
#include "Arrow.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "SceneMgr.h"


CArrow::CArrow()
	: m_ePreScene(DIR::END), m_eCurScene(DIR::END), m_wstrStateKey(L"")
{
}


CArrow::~CArrow()
{
	Release_Object();
}

HRESULT CArrow::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 5.0 };
	m_tUnit = { 5, 0, 0 };

	if (CSceneMgr::Get_Instance()->Get_Scroll())
		m_fSpeed = 20.f;
	else
		m_fSpeed = 10.f;

	return S_OK;
}

int CArrow::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eDir)
	{
	case DIR::LEFT:
		m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		break;
	case DIR::RIGHT:
		m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		break;
	case DIR::UP:
		m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		break;
	case DIR::DOWN:
		m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
		break;
	}

	Frame_Move();
	Scene_Change();


	return OBJ_NOEVENT;
}

void CArrow::LateUpdate_Object()
{
	if (m_tInfo.vPos.x - 5.f <= 0.f || m_tInfo.vPos.x + 5.f >= 1800.f
		|| m_tInfo.vPos.y - 5.f <= 0.f || m_tInfo.vPos.y + 5.f >= 1200.f)
		m_bDead = true;
}

void CArrow::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ARROW", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 2.f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 1.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 2.f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 1.5f));


#ifdef _DEBUG
	// 충돌박스 출력
	D3DXVECTOR2 vLine[5] =
	{
		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y },
		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.top + vScroll.y },
		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.bottom + vScroll.y },
		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.bottom + vScroll.y },
		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y }
	};

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

#endif

}

void CArrow::Release_Object()
{
}

void CArrow::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case DIR::LEFT:
			m_wstrStateKey = L"LEFT";
			break;
		case DIR::RIGHT:
			m_wstrStateKey = L"RIGHT";
			break;
		case DIR::UP:
			m_wstrStateKey = L"UP";
			break;
		case DIR::DOWN:
			m_wstrStateKey = L"DOWN";
			break;
		}
		m_ePreScene = m_eCurScene;
	}

}
