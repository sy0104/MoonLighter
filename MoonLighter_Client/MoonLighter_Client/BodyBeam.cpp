#include "stdafx.h"
#include "BodyBeam.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CBodyBeam::CBodyBeam()
	: m_iAlpha(255)
{
}


CBodyBeam::~CBodyBeam()
{
	Release_Object();
}

HRESULT CBodyBeam::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 10.0 };

	
	return S_OK;
}

int CBodyBeam::Update_Object()
{
	if (m_bDead)
		m_iAlpha -= 10;

	if (m_iAlpha <= 0)
		return OBJ_DEAD;

	m_fAngle -= 1.f;

	Frame_Move();

	return OBJ_NOEVENT;
}

void CBodyBeam::LateUpdate_Object()
{
}

void CBodyBeam::Render_Object()
{
	D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_D", L"BOSS_D_BALL_ATTACK_C", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CBodyBeam::Release_Object()
{
}


void CBodyBeam::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	// 0 ~ 4 นÝบน
	if (!m_bDead && m_tFrame.dFrameStart >= 4.0)
		m_tFrame.dFrameStart = 0.0;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_tFrame.dFrameStart = 0.0;
	}
}
