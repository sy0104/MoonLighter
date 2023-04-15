#include "stdafx.h"
#include "Particle_Bullet.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"



CParticle_Bullet::CParticle_Bullet()
	: m_bDeathMotion(false), m_bGuide(false), m_bCheck(false)
{
}


CParticle_Bullet::~CParticle_Bullet()
{
	Release_Object();
}

HRESULT CParticle_Bullet::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 8.0 };

	m_tUnit = { 5, 0, 0 };

	m_fSpeed = 5.f;



	return S_OK;
}

int CParticle_Bullet::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	// Particle이 생성
	if (m_bGuide)
	{
		if (!m_bCheck)
		{
			CObj* pTarget = CObjMgr::Get_Instance()->Get_Player();

			m_tInfo.vDir = pTarget->Get_Info().vPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			m_bCheck = true;
		}

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}

	// PlantHead가 생성
	else
	{
		m_tInfo.vPos.x += cosf(D3DXToRadian(m_fAngle)) * m_fSpeed;
		m_tInfo.vPos.y -= sinf(D3DXToRadian(m_fAngle)) * m_fSpeed;
	}


	if (m_tInfo.vPos.x - 16.f <= 0.f || m_tInfo.vPos.x + 16.f >= 1800.f
		|| m_tInfo.vPos.y - 16.f <= 0.f || m_tInfo.vPos.y + 16.f >= 1200.f)
		m_bDeathMotion = true;


	return OBJ_NOEVENT;
}

void CParticle_Bullet::LateUpdate_Object()
{
	if (m_bDeathMotion)
	{
		m_fSpeed = 0.f;
		Frame_Move();
	}
}

void CParticle_Bullet::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE", L"BOSS_B_PARTICLE_BULLET", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
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

void CParticle_Bullet::Release_Object()
{
}

void CParticle_Bullet::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd* CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
		m_bDead = true;
}
