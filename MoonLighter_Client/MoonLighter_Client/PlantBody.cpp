#include "stdafx.h"
#include "PlantBody.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "Particle_Bullet.h"
#include "Root.h"
#include "BossHp.h"
#include "BossHpBar.h"


CPlantBody::CPlantBody()
	: m_ePreScene(PLANT::BODY_END), m_eCurScene(PLANT::BODY_END), m_wstrStateKey(L""), m_pTarget(nullptr), m_pBossHp(nullptr), m_pBossHpBar(nullptr)
	, m_dRootTime(0.0), m_bCreateRoot(false), m_bMove(true), m_bJump(false), m_bSetDir(false), m_dHitTime(0.0), m_bInitFrame(false), m_bDeathMotion(false)
{
}


CPlantBody::~CPlantBody()
{
	Release_Object();
}

HRESULT CPlantBody::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 1.f;

	m_wstrStateKey = L"IDLE";

	m_tFrame = { 0.0, 8.0 };
	m_tUnit = { 10, 150, 150 };

	// UI
	// BossHp
	m_pBossHp = CAbstractFactory<CBossHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHp);

	// BossHpBar
	m_pBossHpBar = CAbstractFactory<CBossHpBar>::Create(600.f, 750.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHpBar);

	return S_OK;
}

int CPlantBody::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bDeathMotion && !m_bInitFrame)
	{
		m_tFrame.dFrameStart = 0.0;
		m_eCurScene = PLANT::BODY_DEATH;
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

void CPlantBody::LateUpdate_Object()
{
}

void CPlantBody::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B_BODY", m_wstrStateKey, int(m_tFrame.dFrameStart));

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
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.7f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y);
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.7f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.6f));


#ifdef _DEBUG
	// Hp 출력
	D3DXMATRIX	matHpTrans;

	D3DXMatrixTranslation(&matHpTrans, 500.f, 100.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matHpTrans);

	swprintf_s(m_szHp, L"BodyHP: %d / %d", m_tUnit.m_iHp, m_tUnit.m_iMaxHp);

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

void CPlantBody::Release_Object()
{
}

void CPlantBody::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == PLANT::BODY_STEM)
		{
			//m_dRootTime = 0.0;
			//m_bCreateRoot = false;
			m_eCurScene = PLANT::BODY_IDLE;
		}

		else if (m_eCurScene == PLANT::BODY_JUMP)
		{
			m_bJump = false;
		}

		else if (m_eCurScene == PLANT::BODY_DEATH)
			m_bDead = true;

		m_tFrame.dFrameStart = 0.0;

	}
}

void CPlantBody::Jump_Attack()
{
	if (m_eCurScene == PLANT::BODY_JUMP)
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

void CPlantBody::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case PLANT::BODY_IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 8.0 };
			break;
		case PLANT::BODY_STEM:
			m_wstrStateKey = L"STEM";
			m_tFrame = { 0.0, 48.0 };
			break;
		case PLANT::BODY_JUMP:
			m_wstrStateKey = L"JUMP";
			m_tFrame = { 0.0, 30.0 };
			m_bJump = true;
			break;
		case PLANT::BODY_DEATH:
			m_wstrStateKey = L"DEATH";
			m_tFrame = { 0.0, 61.0 };
			m_dFrameSpeed = 0.1;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CPlantBody::Create_Root()
{
	m_dRootTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	float PlayerX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.x;
	float PlayerY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y;

	float fX = 0.f;
	float fY = 0.f;

	// 1구역: Player - 150 <  < Player + 150
	if (!m_bCreateRoot && m_dRootTime >= 0.0 && m_dRootTime < 0.3)
	{
		for (int i = 0; i < 5; ++i)
		{
			fX = (float)((rand() % (-300)) + (PlayerX - 150));
			fY = (float)((rand() % (-300)) + (PlayerY - 150));

			// 중간구역이면 다시 rand
			if (fX >= PlayerX - 50.f && fX <= PlayerX + 50)
				fX = (float)((rand() % (-300)) + (PlayerX - 150));
			if (fY >= PlayerY - 50.f && fY <= PlayerY + 50)
				fY = (float)((rand() % (-300)) + (PlayerY - 150));

			CObj* pObj = CAbstractFactory<CRoot>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, pObj);
		}

		m_bCreateRoot = true;
	}

	else if (m_bCreateRoot && m_dRootTime >= 0.3 && m_dRootTime < 0.5)
		m_bCreateRoot = false;

	// 2구역: Player - 100 <  < Player + 100
	else if (!m_bCreateRoot && m_dRootTime >= 0.5 && m_dRootTime < 0.8)
	{
		for (int i = 0; i < 5; ++i)
		{
			fX = (float)((rand() % (-200)) + (PlayerX - 100));
			fY = (float)((rand() % (-200)) + (PlayerY - 100));

			// 중간구역이면 다시 rand
			if (fX >= PlayerX - 50.f && fX <= PlayerX + 50)
				fX = (float)((rand() % (-200)) + (PlayerX - 100));
			if (fY >= PlayerY - 50.f && fY <= PlayerY + 50)
				fY = (float)((rand() % (-200)) + (PlayerY - 100));

			CObj* pObj = CAbstractFactory<CRoot>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, pObj);
		}

		m_bCreateRoot = true;
	}

	else if (m_bCreateRoot && m_dRootTime >= 0.8 && m_dRootTime < 1.0)
		m_bCreateRoot = false;

	// 3구역: Player - 50 <  < Player + 50 (중간구역 포함)
	else if (!m_bCreateRoot && m_dRootTime >= 1.0 && m_dRootTime < 1.3)
	{
		for (int i = 0; i < 5; ++i)
		{
			fX = (float)((rand() % (-50)) + (PlayerX - 25));
			fY = (float)((rand() % (-50)) + (PlayerY - 25));

			CObj* pObj = CAbstractFactory<CRoot>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, pObj);
		}

		m_bCreateRoot = true;
	}
}