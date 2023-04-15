#include "stdafx.h"
#include "Ball.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"


CBall::CBall()
	: m_pTarget(nullptr), m_bCheck(false), m_iAlpha(255)
{
}


CBall::~CBall()
{
	Release_Object();
}

HRESULT CBall::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 3.f, 3.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 6.0 };


	return S_OK;
}

int CBall::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCheck)
	{
		m_pTarget = CObjMgr::Get_Instance()->Get_Player();

		m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_bCheck = true;
	}

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;


	Frame_Move();

	return OBJ_NOEVENT;
}

void CBall::LateUpdate_Object()
{

}

void CBall::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_D", L"BOSS_D_BALL_ATTACK_A", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// RECT
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));


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

void CBall::Release_Object()
{
}
