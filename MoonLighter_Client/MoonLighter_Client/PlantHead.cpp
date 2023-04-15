#include "stdafx.h"
#include "PlantHead.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "Particle_Bullet.h"
#include "ParticleSeed.h"
#include "Item.h"
#include "BossHp.h"
#include "BossHpBar.h"

#include "SoundMgr.h"

CPlantHead::CPlantHead()
	: m_ePreScene(PLANT::HEAD_END), m_eCurScene(PLANT::HEAD_END), m_wstrStateKey(L""), m_pTarget(nullptr), m_pBossHp(nullptr), m_pBossHpBar(nullptr)
	, m_dBulletTime(0.0), m_bMove(true), m_bJump(false), m_bSetDir(false), m_bCreateSeed(false), m_dHitTime(0.0), m_bDeathMotion(false), m_bInitFrame(false)
	, m_dFrameSpeed(0.5)
{
}


CPlantHead::~CPlantHead()
{
	Release_Object();
}

HRESULT CPlantHead::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_wstrStateKey = L"IDLE";

	m_tFrame = { 0.0, 8.0 };
	m_tUnit = { 10, 150, 150 };

	m_fSpeed = 1.f;

	m_fAngle1 = 0.f;
	m_fAngle2 = 120.f;
	m_fAngle3 = 240.f;

	// UI
	// BossHp
	m_pBossHp = CAbstractFactory<CBossHp>::Create(1400.f, 700.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHp);

	// BossHpBar
	m_pBossHpBar = CAbstractFactory<CBossHpBar>::Create(600.f, 700.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHpBar);

	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"Golem2.wav");


	return S_OK;
}

int CPlantHead::Update_Object()
{
	if (m_bDead)
	{
		for (int i = 0; i < 20; ++i)
		{
			float fX = (float)((rand() % (-600)) + (m_tInfo.vPos.x - 300.f));
			float fY = (float)((rand() % (-400)) + (m_tInfo.vPos.y - 200.f));

			CObj* pItem = CAbstractFactory<CItem>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, pItem);
			dynamic_cast<CItem*>(pItem)->Set_StateKey(L"Plant_Items");
		}

		return OBJ_DEAD;
	}

	if (m_bDeathMotion && !m_bInitFrame)
	{
		m_tFrame.dFrameStart = 0.0;
		m_eCurScene = PLANT::HEAD_DEATH;
		m_bInitFrame = true;
		m_bMove = false;
	}

	// 플레이어 따라감
	if (m_bMove)
	{
		CObj* pTarget = CObjMgr::Get_Instance()->Get_Instance()->Get_Player();

		m_tInfo.vDir = pTarget->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}

	// 피격
	if (m_bAlpha)
	{
		m_dHitTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dHitTime >= 0.5)
		{
			m_bAlpha = false;
			m_dHitTime = 0.0;
		}
	}


	// Hp Bar
	if (m_tUnit.m_iHp >= 0)
		dynamic_cast<CBossHp*>(m_pBossHp)->Set_HpRect((LONG)(m_tUnit.m_iHp));


	Jump_Attack();

	Frame_Move(m_dFrameSpeed);
	Scene_Change();

	return OBJ_NOEVENT;
}

void CPlantHead::LateUpdate_Object()
{

}

void CPlantHead::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_HEAD", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	if (!m_bAlpha)
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y);


#ifdef _DEBUG
	// Hp 출력
	D3DXMATRIX	matHpTrans;

	D3DXMatrixTranslation(&matHpTrans, 500.f, 50.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matHpTrans);

	swprintf_s(m_szHp, L"HeadHP: %d / %d", m_tUnit.m_iHp, m_tUnit.m_iMaxHp);

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

void CPlantHead::Release_Object()
{
}

void CPlantHead::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_eCurScene == PLANT::HEAD_DEATH)
		CSoundMgr::Get_Instance()->PlaySound(L"Plant_Death.wav", CSoundMgr::EFFECT);


	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == PLANT::HEAD_STARTSHAKE)
			m_eCurScene = PLANT::HEAD_CYCLESHAKE;

		else if (m_eCurScene == PLANT::HEAD_ENDSHAKE || m_eCurScene == PLANT::HEAD_POTS)
			m_eCurScene = PLANT::HEAD_IDLE;

		else if (m_eCurScene == PLANT::HEAD_JUMP)
		{
			m_bJump = false;
		}

		else if (m_eCurScene == PLANT::HEAD_DEATH)
			m_bDead = true;

		m_tFrame.dFrameStart = 0.0;
	}
}

void CPlantHead::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case PLANT::HEAD_IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 8.0 };
			m_dFrameSpeed = 0.5;
			break;
		case PLANT::HEAD_POTS:
			m_wstrStateKey = L"POTS";
			m_tFrame = { 0.0, 40.0 };
			m_dFrameSpeed = 0.25;
			break;
		case PLANT::HEAD_JUMP:
			m_bJump = true;
			m_wstrStateKey = L"JUMP";
			m_tFrame = { 0.0, 30.0 };
			m_dFrameSpeed = 0.5;
			break;
		case PLANT::HEAD_STARTSHAKE:
			m_wstrStateKey = L"STARTSHAKE";
			m_tFrame = { 0.0, 28.0 };
			m_dFrameSpeed = 0.5;
			break;
		case PLANT::HEAD_CYCLESHAKE:
			m_wstrStateKey = L"CYCLESHAKE";
			m_tFrame = { 0.0, 16.0 };
			m_dFrameSpeed = 0.5;
			break;
		case PLANT::HEAD_ENDSHAKE:
			m_wstrStateKey = L"ENDSHAKE";
			m_tFrame = { 0.0, 12.0 };
			m_dFrameSpeed = 0.5;
			break;
		case PLANT::HEAD_DEATH:
			m_wstrStateKey = L"DEATH";
			m_tFrame = { 0.0, 90.0 };
			m_dFrameSpeed = 0.1;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CPlantHead::Create_Particle()
{
	CSoundMgr::Get_Instance()->PlaySound(L"Plant_Shot.wav", CSoundMgr::EFFECT);

	// 1. FrameStart == 16일때 생성
	if (m_tFrame.dFrameStart >= 16.0 && m_tFrame.dFrameStart < 17.0)
	{
		if (!m_bCreateSeed)
		{
			CObj* pObj = CAbstractFactory<CParticleSeed>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::BACK, pObj);
			dynamic_cast<CParticleSeed*>(pObj)->Set_PosY(700.f);

			m_bCreateSeed = true;
		}
	}

	else if (m_tFrame.dFrameStart >= 17.0 && m_tFrame.dFrameStart < 18.0)
		m_bCreateSeed = false;

	// 2. FrameStart == 22일때 생성
	if (m_tFrame.dFrameStart >= 22.0 && m_tFrame.dFrameStart < 23.0)
	{
		if (!m_bCreateSeed)
		{
			CObj* pObj = CAbstractFactory<CParticleSeed>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::BACK, pObj);
			dynamic_cast<CParticleSeed*>(pObj)->Set_PosY(200.f);

			m_bCreateSeed = true;
		}
	}

	else if (m_tFrame.dFrameStart >= 23.0 && m_tFrame.dFrameStart < 24.0)
		m_bCreateSeed = false;
	
	// 3. FrameStart == 28일때 생성
	if (m_tFrame.dFrameStart >= 28.0 && m_tFrame.dFrameStart < 29.0)
	{
		if (!m_bCreateSeed)
		{
			CObj* pObj = CAbstractFactory<CParticleSeed>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::BACK, pObj);
			dynamic_cast<CParticleSeed*>(pObj)->Set_PosY(1000.f);

			m_bCreateSeed = true;
		}
	}

	else if (m_tFrame.dFrameStart >= 29.0 && m_tFrame.dFrameStart < 30.0)
		m_bCreateSeed = false;
}

void CPlantHead::Jump_Attack()
{
	if (m_eCurScene == PLANT::HEAD_JUMP)
	{
		if (m_tFrame.dFrameStart >= 11.0 && m_tFrame.dFrameStart <= 18)
		{
			if (!m_bSetDir)
			{
				m_pTarget = CObjMgr::Get_Instance()->Get_Instance()->Get_Player();

				m_bSetDir = true;
			}

			m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;
			m_tInfo.vPos += m_tInfo.vDir * 0.05f;
		}

		else if (m_tFrame.dFrameStart > 18 && m_tFrame.dFrameStart <= m_tFrame.dFrameEnd)
		{
			m_bSetDir = false;
		}
	}
}


void CPlantHead::Create_Bullet()
{
	m_dBulletTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	//CSoundMgr::Get_Instance()->PlaySound(L"Plant_Idle.wav", CSoundMgr::EFFECT);

	if (m_dBulletTime > 0.2)
	{
		m_fAngle1 += 5.f;
		m_fAngle2 += 5.f;
		m_fAngle3 += 5.f;

		CObj* pObj = CAbstractFactory<CParticle_Bullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fAngle1);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, pObj);

		pObj = CAbstractFactory<CParticle_Bullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fAngle2);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, pObj);

		pObj = CAbstractFactory<CParticle_Bullet>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y, m_fAngle3);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, pObj);

		m_dBulletTime = 0.0;
	}

}
