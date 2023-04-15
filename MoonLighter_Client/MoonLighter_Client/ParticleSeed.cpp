#include "stdafx.h"
#include "ParticleSeed.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "ExtraParticle.h"


CParticleSeed::CParticleSeed()
	: m_dFlyTime(0.0), m_dFallTime(0.0), m_eDir(END), m_iRand(0), m_fSpeedX(0.f), m_fSpeedY(0.f), m_fPosY(0.f)
{
}


CParticleSeed::~CParticleSeed()
{
	Release_Object();
}

HRESULT CParticleSeed::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iRand = rand() % 2;

	m_fSpeedX = (float)(rand() % 2 + 1);
	m_fSpeedY = (float)(rand() % 2 + 2);

	m_dFallTime = (double)(rand() % 2 + 1);

	if (m_iRand == 0)
		m_eDir = LEFT;
	else
		m_eDir = RIGHT;

	m_fSpeed = 3.f;

	return S_OK;
}

int CParticleSeed::Update_Object()
{
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CExtraParticle>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);


		return OBJ_DEAD;
	}

	m_dFlyTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dFlyTime >= 0.0 && m_dFlyTime < m_dFallTime)
	{
		if (m_eDir == LEFT)
		{
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeedX;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeedY;
		}

		else
		{
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeedX;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeedY;
		}
	}

	else if (m_dFlyTime >= m_dFallTime)
	{
		if (m_tInfo.vPos.y <= m_fPosY)
		{
			if (m_fSpeedX >= 0.5f)
				m_fSpeedX -= 0.001f;

			if (m_eDir == LEFT)
			{
				m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeedX;
				m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeedY;
			}

			else
			{
				m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeedX;
				m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeedY;
			}
		}

		else
			m_bDead = true;
	}

	return OBJ_NOEVENT;
}

void CParticleSeed::LateUpdate_Object()
{
}

void CParticleSeed::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;
	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE", L"BOSS_B_PARTICLE_SEED", 0);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);


	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CParticleSeed::Release_Object()
{
}
