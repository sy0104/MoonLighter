#include "stdafx.h"
#include "ParticleHead.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "Particle_Bullet.h"
#include "DeathParticle.h"


CParticleHead::CParticleHead()
	: m_ePreScene(PARTICLE::HEAD_END), m_eCurScene(PARTICLE::HEAD_END), m_wstrStateKey(L""), m_dBulletTime(0.0)
	, m_bCreateBullet(false), m_dHitTime(0.0), m_pParticle_Body(nullptr)
{
}


CParticleHead::~CParticleHead()
{
	Release_Object();
}

HRESULT CParticleHead::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_ePreScene = PARTICLE::HEAD_IDLE;
	m_eCurScene = PARTICLE::HEAD_IDLE;
	m_wstrStateKey = L"IDLE";

	m_tFrame = { 0.0, 6.0 };

	m_tUnit = { 5, 20, 20 };



	return S_OK;
}

int CParticleHead::Update_Object()
{
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CDeathParticle>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BACK, pObj);

		return OBJ_DEAD;
	}

	if (m_eCurScene == PARTICLE::HEAD_SHOT && m_tFrame.dFrameStart >= 11.0)
	{
		if (!m_bCreateBullet)
		{
			CObj* pObj = CAbstractFactory<CParticle_Bullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fAngle);
			CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, pObj);
			dynamic_cast<CParticle_Bullet*>(pObj)->Set_Guide();

			m_bCreateBullet = true;
		}
	}

	// 피격
	if (m_bAlpha)
	{
		m_iAlpha = 150;

		m_dHitTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dHitTime >= 0.5)
		{
			m_bAlpha = false;
			m_dHitTime = 0.0;
		}
	}
	else
		m_iAlpha = 255;



	// 죽음
	if (m_tUnit.m_iHp <= 0)
	{
		m_bDead = true;
	}

	Frame_Move();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CParticleHead::LateUpdate_Object()
{
	float fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.x - m_tInfo.vPos.x;
	float fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y - m_tInfo.vPos.y;

	float fDia = sqrtf(fX * fX + fY * fY);

	float fRad = acosf(fX / fDia);
	m_fAngle = D3DXToDegree(fRad);

	if (m_tInfo.vPos.y > CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y)
		m_fAngle *= -1;
}

void CParticleHead::Render_Object()
{
	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE_HEAD", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));


#ifdef _DEBUG

	// Hp 출력
	D3DXMATRIX	matHpTrans;

	D3DXMatrixTranslation(&matHpTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matHpTrans);

	swprintf_s(m_szHp, L"HP: %d / %d", m_tUnit.m_iHp, m_tUnit.m_iMaxHp);

	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szHp, lstrlen(m_szHp), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


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

void CParticleHead::Release_Object()
{
}

void CParticleHead::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd* CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == PARTICLE::HEAD_IDLE)
			m_eCurScene = PARTICLE::HEAD_SHOT;
		else if (m_eCurScene = PARTICLE::HEAD_SHOT)
		{
			m_bCreateBullet = false;
			m_eCurScene = PARTICLE::HEAD_IDLE;
		}

		m_tFrame.dFrameStart = 0.0;
	}

}

void CParticleHead::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case PARTICLE::HEAD_IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 6.0 };
			break;
		case PARTICLE::HEAD_SHOT:
			m_wstrStateKey = L"SHOT";
			m_tFrame = { 0.0, 15.0 };
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CParticleHead::Progress()
{
	m_dBulletTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dBulletTime >= 0.0 && m_dBulletTime < 2.0)
	{
		m_eCurScene = PARTICLE::HEAD_IDLE;
	}

	else if (m_dBulletTime >= 2.0)
	{
		m_eCurScene = PARTICLE::HEAD_SHOT;

		if (m_tFrame.dFrameStart == 11.0)
		{ 
			CObj* pObj = CAbstractFactory<CParticle_Bullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fAngle);
			CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, pObj);

			m_dBulletTime = 0.0;
		}
	}
}
