#include "stdafx.h"
#include "GolemKing.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "Rock.h"
#include "Shadow.h"
#include "Effect.h"
#include "Ui.h"
#include "Item.h"
#include "SoundMgr.h"

#include "BossHp.h"
#include "BossHpBar.h"


CGolemKing::CGolemKing()
	: m_ePreScene(END), m_eCurScene(END), m_wstrStateKey(L""), m_dFrameSpeed(0.3), m_bIntro(false), m_bWaveEffect(false), m_dWaveTime(0.0)
	, m_bCreateRockShadow(false), m_bCreateRock1(false), m_bCreateRock2(true), m_bCreateRock3(true), m_bCreateRandomRock(false), m_fRockDis(0.f), m_dRockTime(0.0), m_fRockAngle(0.f)
	, m_bLaunch(false), m_bRecover(false), m_bCreatePunchShadow(false), m_iLaunchCnt(0), m_dHitTime(0.0), m_bDeadMotion(false), m_bInitFrame(false)
	, m_pBossHp(nullptr), m_pBossHpBar(nullptr)
{
}


CGolemKing::~CGolemKing()
{
	Release_Object();
}

HRESULT CGolemKing::Ready_Object()
{
	m_tInfo.vPos = { 900.f, 350.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_ePreScene = INTRO;
	m_eCurScene = INTRO;
	m_wstrStateKey = L"WAKEUP";
	m_tFrame = { 0.0, 32.0 };

	m_tUnit = { 10, 150, 150 };

	m_fRockAngle = 190.f;
	m_fRockDis = 300.f;

	// UI
	// BossHp
	m_pBossHp = CAbstractFactory<CBossHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHp);

	// BossHpBar
	m_pBossHpBar = CAbstractFactory<CBossHpBar>::Create(600.f, 750.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHpBar);

	// Wakeup Sound
	CSoundMgr::Get_Instance()->PlaySound(L"Golem_WakeUp.wav", CSoundMgr::EFFECT);

	return S_OK;
}

int CGolemKing::Update_Object()
{
	if (m_bDead)
	{
		// Item
		for (int i = 0; i < 20; ++i)
		{
			CObj* pItem = CAbstractFactory<CItem>::Create((float)(rand() % 800 + 400), (float)(rand() % 200 + 350));
			CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, pItem);
			dynamic_cast<CItem*>(pItem)->Set_StateKey(L"Golem_Items");
		}


		return OBJ_DEAD;
	}


	// 인트로
	if (m_bIntro && !m_bDead)
	{
		Progress();
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


	// 죽음
	if (m_tUnit.m_iHp <= 0)
	{
		if (!m_bInitFrame)
		{
			m_tFrame.dFrameStart = 0.0;
			m_dFrameSpeed = 0.2;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
			m_bInitFrame = true;
		}
		m_eCurScene = DEATH;
	}

	// Hp Bar
	if (m_tUnit.m_iHp >= 0)
		dynamic_cast<CBossHp*>(m_pBossHp)->Set_HpRect((LONG)(m_tUnit.m_iHp));

	Frame_Move(m_dFrameSpeed);
	Scene_Change();


	return OBJ_NOEVENT;
}

void CGolemKing::LateUpdate_Object()
{
}

void CGolemKing::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_A", m_wstrStateKey, int(m_tFrame.dFrameStart));

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
	m_tRect.left = (LONG)(m_tInfo.vPos.x - fCenterX);
	m_tRect.top = (LONG)(m_tInfo.vPos.y - fCenterY);
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.8f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.3f));


#ifdef _DEBUG
	// Wave 시간 출력
	D3DXMATRIX	matFontTrans;
	D3DXMatrixTranslation(&matFontTrans, 100.f, 50.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);

	swprintf_s(m_szWaveTime, L"Time: %f", m_dWaveTime);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szWaveTime, lstrlen(m_szWaveTime), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Hp 출력
	D3DXMATRIX	matHpTrans;
	D3DXMatrixTranslation(&matHpTrans, 600.f, 50.f, 0.f);
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

void CGolemKing::Release_Object()
{
}

void CGolemKing::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_eCurScene == SPAWN && m_tFrame.dFrameStart >= 25.0)
		CSoundMgr::Get_Instance()->PlaySound(L"Golem_Spawn.wav", CSoundMgr::EFFECT);

	else if (m_eCurScene == DEATH)
		CSoundMgr::Get_Instance()->PlaySound(L"Golem_Death.wav", CSoundMgr::EFFECT);


	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == INTRO)
		{
			//m_dFrameSpeed = 1.0;
			m_bIntro = true;
			m_eCurScene = IDLE;
			CSoundMgr::Get_Instance()->PlayBGM(L"Golem1.wav");
		}

		else if (m_eCurScene == SPAWN || m_eCurScene == RECOVER)
			m_eCurScene = IDLE;

		else if (m_eCurScene == LAUNCH)
			m_eCurScene = NOARM_IDLE;

		else if (m_eCurScene == DEATH)
			m_bDead = true;


		m_tFrame.dFrameStart = 0.0;
	}
}

void CGolemKing::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CGolemKing::INTRO:
			m_wstrStateKey = L"WAKEUP";
			m_tFrame = { 0.0, 32.0 };
			m_dFrameSpeed = 0.3;
			break;
		case CGolemKing::IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 16.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CGolemKing::SPAWN:
			m_wstrStateKey = L"SPAWN";
			m_tFrame = { 0.0, 43.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CGolemKing::LAUNCH:
			m_wstrStateKey = L"LAUNCH";
			m_tFrame = { 0.0, 39.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CGolemKing::NOARM_IDLE:
			m_wstrStateKey = L"NOARM_IDLE";
			m_tFrame = { 0.0, 16.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CGolemKing::RECOVER:
			m_wstrStateKey = L"RECOVER";
			m_tFrame = { 0.0, 24.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CGolemKing::DEATH:
			m_wstrStateKey = L"DEATH";
			m_tFrame = { 0.0, 80.0 };
			m_dFrameSpeed = 0.2;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CGolemKing::Initialize()
{
	// Spawn
	m_bCreateRockShadow = false;
	m_bCreateRock1 = false;
	m_bCreateRock2 = true;
	m_bCreateRock3 = true;
	m_bCreateRandomRock = false;
	m_fRockAngle = 190.f;
	m_fRockDis = 300.f;
	m_dRockTime = 0.0;

	// Launch
	m_bLaunch = false;
	m_bRecover = false;
	m_bCreatePunchShadow = false;
	m_iLaunchCnt = 0;

	// Effect
	m_bWaveEffect = false;
}

void CGolemKing::Progress()
{
	m_dWaveTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	// Wave1: 부채꼴로 돌 떨구기
	if (m_dWaveTime >= 3.0 && m_dWaveTime < 10.0)
	{
		Spawn_Shadow();
		Spawn_Rock();
	}

	// Wave_Effect
	else if (m_dWaveTime >= 10.0 && m_dWaveTime < 15.0)
	{
		Create_WaveEffect();
	}

	// Wave2: 팔 떨구기
	else if (m_dWaveTime >= 15.0 && m_dWaveTime < 25.0)
	{
		m_bWaveEffect = false;
		Launch();

		if (m_dWaveTime >= 19.0 && !m_bRecover)
		{
			m_eCurScene = RECOVER;
			m_bRecover = true;
		}
	}

	// Wave Effect
	else if (m_dWaveTime >= 25.0 && m_dWaveTime < 28.0)
		Create_WaveEffect();


	// Wave3: 랜덤하게 돌 떨구기
	else if (m_dWaveTime >= 28.0 && m_dWaveTime < 38.0)
	{
		m_bWaveEffect = false;

		Spawn_Rock_Random();

		// Launch 초기화
		m_bLaunch = false;
		m_bCreatePunchShadow = false;
		m_iLaunchCnt = 0;
		m_bRecover = false;
	}

	// Wave Effect
	else if (m_dWaveTime >= 38.0 && m_dWaveTime < 41.0)
		Create_WaveEffect();

	// Wave4: 팔 떨구기
	else if (m_dWaveTime >= 41.0 && m_dWaveTime < 51.0)
	{
		Launch();

		if (m_dWaveTime >= 47.0 && !m_bRecover)
		{
			m_eCurScene = RECOVER;
			m_bRecover = true;
		}
	}

	else if (m_dWaveTime >= 51.0)
	{
		Initialize();
		m_dWaveTime = 0.0;
	}
}

void CGolemKing::Spawn_Shadow()
{
	if (!m_bCreateRockShadow)
	{
		m_eCurScene = SPAWN;

		if (m_tFrame.dFrameStart >= 25.0)
		{


			//// 1단 ////
			for (int i = 0; i < 14; ++i)
			{
				m_vShadowPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
				m_vShadowPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);

				m_fRockAngle += 12.f;

				if (i == 13)
					m_fRockAngle = 185.f;
			}

			//// 2단 ////
			for (int i = 0; i < 25; ++i)
			{
				m_fRockDis = 500.f;
				m_vShadowPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
				m_vShadowPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);

				m_fRockAngle += 7.f;

				if (i == 24)
					m_fRockAngle = 185.f;
				
			}


			//// 3단 ////
			for (int i = 0; i < 35; ++i)
			{
				m_fRockDis = 700.f;
				m_vShadowPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
				m_vShadowPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);

				m_fRockAngle += 5.f;

				if (i == 34)
				{
					m_fRockAngle = 190.f;
					m_fRockDis = 300.f;
				}
			}

			m_bCreateRockShadow = true;
		}
	}
}

void CGolemKing::Spawn_Rock()
{
	if (m_bCreateRockShadow)
	{
		m_dRockTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	}

	//// 1단 ////
	if (!m_bCreateRock1 && m_dRockTime > 2.0)
	{
		for (int i = 0; i < 14; ++i)
		{
			m_vRockPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
			m_vRockPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis) - 200.f;

			CObj* pObj = CAbstractFactory<CRock>::Create(m_vRockPos.x, m_vRockPos.y);
			CObjMgr::Get_Instance()->Add_Object(OBJ::ROCK, pObj);
			dynamic_cast<CRock*>(pObj)->Set_PosY(m_vRockPos.y + 200.f);

			m_fRockAngle += 12.f;
			if (i == 13)
			{
				m_fRockAngle = 185.f;
				m_bCreateRock1 = true;
				m_bCreateRock2 = false;
			}
		}
	}

	//// 2단 ////
	else if (!m_bCreateRock2)
	{
		m_dRockTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dRockTime > 0.1)
		{
			m_fRockDis = 500.f;

			for (int i = 0; i < 25; ++i)
			{
				m_vRockPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
				m_vRockPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis) - 150.f;

				CObj* pObj = CAbstractFactory<CRock>::Create(m_vRockPos.x, m_vRockPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::ROCK, pObj);
				dynamic_cast<CRock*>(pObj)->Set_PosY(m_vRockPos.y + 150.f);

				m_fRockAngle += 7.f;
				if (i == 24)
				{
					m_fRockAngle = 185.f;
					m_dRockTime = 0.0;
					m_bCreateRock2 = true;
					m_bCreateRock3 = false;
				}
			}
		}
	}

	//// 3단 ////
	else if (!m_bCreateRock3)
	{
		m_dRockTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dRockTime > 0.1)
		{
			m_fRockDis = 700.f;

			for (int i = 0; i < 35; ++i)
			{
				m_vRockPos.x = (m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fRockAngle))) * m_fRockDis);
				m_vRockPos.y = (m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fRockAngle))) * m_fRockDis) - 100.f;

				CObj* pObj = CAbstractFactory<CRock>::Create(m_vRockPos.x, m_vRockPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::ROCK, pObj);
				dynamic_cast<CRock*>(pObj)->Set_PosY(m_vRockPos.y + 100.f);

				m_fRockAngle += 5.f;
				if (i == 34)
					m_bCreateRock3 = true;
			}
		}
	}
}

void CGolemKing::Spawn_Rock_Random()
{
	if (!m_bCreateRandomRock)
	{
		m_eCurScene = SPAWN;

		if (m_tFrame.dFrameStart >= 25.0)
		{

			// 1구역: x: 100 ~ 600, y: 350 ~ 950
			for (int i = 0; i < 20; ++i)
			{
				m_vShadowPos.x = (float)(rand() % 500 + 100);
				m_vShadowPos.y = (float)(rand() % 650 + 350);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_RandomRock();
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);
			}

			// 2구역: x: 650 ~ 1100, y: 500 ~ 1000
			for (int i = 0; i < 20; ++i)
			{
				m_vShadowPos.x = (float)(rand() % 450 + 650);
				m_vShadowPos.y = (float)(rand() % 500 + 500);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_RandomRock();
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);
			}
			
			// 3구역: x: 1200 ~ 1500, y: 350 ~ 950
			for (int i = 0; i < 20; ++i)
			{
				m_vShadowPos.x = (float)(rand() % 300 + 1200);
				m_vShadowPos.y = (float)(rand() % 650 + 350);

				CObj* pObj = CAbstractFactory<CShadow>::Create(m_vShadowPos.x, m_vShadowPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
				dynamic_cast<CShadow*>(pObj)->Set_RandomRock();
				dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"ROCK_SHADOW");
				pObj->Set_Size(1.5f, 1.5f);
			}


			m_bCreateRandomRock = true;

		}
	}
}

void CGolemKing::Launch()
{
	if (!m_bLaunch)
	{
		m_eCurScene = LAUNCH;
		m_tFrame.dFrameStart = 0.0;
		m_bLaunch = true;
	}

	else if (m_eCurScene == NOARM_IDLE && !m_bCreatePunchShadow)
	{
		D3DXVECTOR2 vPos = { CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.x,
							 CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y };

		float fX = (float)(rand() % (-500)) + (vPos.x - 250.f);
		float fY = (float)(rand() % (-500)) + (vPos.y - 250.f);


		CObj* pObj = CAbstractFactory<CShadow>::Create(fX, fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ::SHADOW, pObj);
		dynamic_cast<CShadow*>(pObj)->Set_Shadow(L"PUNCH_SHADOW");
		pObj->Set_Size(0.2f, 0.2f);

		m_bCreatePunchShadow = true;
	}
}

void CGolemKing::Create_WaveEffect()
{
	CSoundMgr::Get_Instance()->PlaySound(L"Golem_Wave.wav", CSoundMgr::EFFECT);

	if (!m_bWaveEffect)
	{
		CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::EFFECT, pObj);
		dynamic_cast<CEffect*>(pObj)->Set_EffectID(EFFECT::WAVE);

		m_bWaveEffect = true;
	}
}
