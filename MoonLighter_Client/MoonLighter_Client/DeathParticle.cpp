#include "stdafx.h"
#include "DeathParticle.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"


CDeathParticle::CDeathParticle()
{
}


CDeathParticle::~CDeathParticle()
{
	Release_Object();
}

HRESULT CDeathParticle::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 15.0 };

	return S_OK;
}

int CDeathParticle::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	

	Frame_Move();

	return OBJ_NOEVENT;
}

void CDeathParticle::LateUpdate_Object()
{
}

void CDeathParticle::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE", L"BOSS_B_PARTICLE_DEATH", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CDeathParticle::Release_Object()
{
}

void CDeathParticle::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_bDead = true;
	}
}
