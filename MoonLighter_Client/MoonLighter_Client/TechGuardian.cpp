#include "stdafx.h"
#include "TechGuardian.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "Ball.h"
#include "Beam.h"
#include "BodyBeam.h"
#include "OutBeam.h"
#include "Effect.h"
#include "ElectricPart.h"
#include "Item.h"
#include "BossHp.h"
#include "BossHpBar.h"
#include "SoundMgr.h"


CTechGuardian::CTechGuardian()
	: m_ePreScene(END), m_eCurScene(END), m_wstrStateKey(L""), m_dWaveTime(0.0), m_bTeleport(false), m_bIntro(false), m_dFrameSpeed(0.09), m_bAttack(false)
	, m_bBallAttack(false), m_bCreateBalls(false), m_pBodyBeam(nullptr), m_pOutBeam(nullptr), m_pBeam(nullptr)
	, m_bDash(true), m_pTarget(false), m_bSetTarget(false), m_iDashCnt(0), m_bDashEffect(false), m_bInitDeath(false)
	, m_bCreateCircle(false), m_bCreatePart(false), m_fPartAngle(0.f), m_fPartDis(0.f), m_dPartTime(0.0), m_bInitFrame(false), m_dHitTime(0.0)
	, m_pBossHp(nullptr), m_pBossHpBar(nullptr)
{
}


CTechGuardian::~CTechGuardian()
{
	Release_Object();
}

HRESULT CTechGuardian::Ready_Object()
{
	m_tInfo.vPos = { 1200.f, 350.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_ePreScene = WAKEUP;
	m_eCurScene = WAKEUP;
	m_wstrStateKey = L"BOSS_D_WAKEUP";
	m_tFrame = { 0.0, 121.0 };

	m_tUnit = { 10, 150, 150 };

	m_fSpeed = 1.f;

	// UI
	// BossHp
	m_pBossHp = CAbstractFactory<CBossHp>::Create(1400.f, 700.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHp);

	// BossHpBar
	m_pBossHpBar = CAbstractFactory<CBossHpBar>::Create(600.f, 700.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pBossHpBar);

	// Sound
	//CSoundMgr::Get_Instance()->PlayBGM(L"TechBGM.wav");
	//CSoundMgr::Get_Instance()->PlayBGM(L"Tech.wav");

	CSoundMgr::Get_Instance()->PlaySound(L"Tech_WakeUp.wav", CSoundMgr::EFFECT);

	return S_OK;
}

int CTechGuardian::Update_Object()
{
	if (m_bDead)
	{
		for (int i = 0; i < 20; ++i)
		{
			float fX = (float)((rand() % (-600)) + (m_tInfo.vPos.x - 300.f));
			float fY = (float)((rand() % (-400)) + (m_tInfo.vPos.y - 200.f));

			CObj* pItem = CAbstractFactory<CItem>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, pItem);
			dynamic_cast<CItem*>(pItem)->Set_StateKey(L"Tech_Items");
		}

		return OBJ_DEAD;
	}



	if (m_bIntro && m_tUnit.m_iHp > 0)
	{
		m_dWaveTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dWaveTime >= 3.0 && m_dWaveTime < 6.0)
		{
			if (!m_bTeleport)
			{
				if (!m_bInitFrame)
				{
					m_tFrame.dFrameStart = 0.0;
					m_bInitFrame = true;
				}

				Teleport(1200.f, 600.f);
			}
		}

		// Ball Attack
		else if (m_dWaveTime >= 6.0 && m_dWaveTime < 15.0)
		{
			m_bTeleport = false;
			Ball_Attack();
		}

		else if (m_dWaveTime >= 15.0 && m_dWaveTime < 20.0)
		{

			m_pBodyBeam->Set_Dead();
			m_pOutBeam->Set_Dead();
			m_pBeam->Set_Dead();

			if (!m_bTeleport)
			{
				if (!m_bInitFrame)
				{
					m_tFrame.dFrameStart = 0.0;
					m_bInitFrame = true;
				}

				Teleport(700.f, 800.f);
			}
		}

		// Dash
		else if (m_dWaveTime >= 20.0 && m_dWaveTime < 30.0)
		{
			m_bTeleport = false;

			if (!m_bAttack)
				Dash();
		}

		else if (m_dWaveTime >= 30.0 && m_dWaveTime < 35.0)
		{
			m_bAttack = false;

			if (!m_bTeleport)
			{
				if (!m_bInitFrame)
				{
					m_tFrame.dFrameStart = 0.0;
					m_bInitFrame = true;
				}
				Teleport(1200.f, 300.f);
			}
		}

		// Electric Part - DOWN
		else if (m_dWaveTime >= 35.0 && m_dWaveTime < 45.0)
		{
			m_bTeleport = false;

			if (!m_bAttack)
				Electric_Part(4);
		}

		else if (m_dWaveTime >= 45.0 && m_dWaveTime < 50.0)
		{
			m_bAttack = false;

			m_fPartAngle = 0.f;
			m_bCreateCircle = false;
			m_bCreatePart = false;
			m_fPartDis = 30.f;
			m_dPartTime = 0.0;

			if (!m_bTeleport)
			{
				if (!m_bInitFrame)
				{
					m_tFrame.dFrameStart = 0.0;
					m_bInitFrame = true;
				}

				Teleport(600.f, 750.f);
			}
		}

		// Electric Part - LEFT
		else if (m_dWaveTime >= 50.0 && m_dWaveTime < 60.0)
		{
			m_bTeleport = false;

			if (!m_bAttack)
				Electric_Part(1);
		}

		// Initialize
		else if (m_dWaveTime >= 60.0)
		{
			Initialize();
			m_dWaveTime = 0.0;
		}
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
		if (m_eCurScene == BALL_ATTACK)
		{
			m_pBodyBeam->Set_Dead();
			m_pOutBeam->Set_Dead();
			m_pBeam->Set_Dead();
		}

		if (!m_bInitDeath)
		{
			m_tFrame.dFrameStart = 0.0;
			m_dFrameSpeed = 0.5;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
			m_bInitDeath = true;
		}
		m_eCurScene = DEATH;
		CSoundMgr::Get_Instance()->PlaySound(L"Tech_Death.wav", CSoundMgr::EFFECT);
	}


	// Hp Bar
	if (m_tUnit.m_iHp >= 0)
		dynamic_cast<CBossHp*>(m_pBossHp)->Set_HpRect((LONG)(m_tUnit.m_iHp));


	Frame_Move(m_dFrameSpeed);
	Scene_Change();

	return OBJ_NOEVENT;
}

void CTechGuardian::LateUpdate_Object()
{

}

void CTechGuardian::Render_Object()
{
	D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_D", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	if (!m_bAlpha)
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));


	// RECT
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));


#ifdef _DEBUG
	// Wave 시간 출력
	D3DXMATRIX	matFontTrans, matPosTrans, matHpTrans;

	D3DXMatrixTranslation(&matFontTrans, 50.f, 50.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matFontTrans);

	swprintf_s(m_szWaveTime, L"Time: %f", m_dWaveTime);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szWaveTime, lstrlen(m_szWaveTime), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Pos 출력
	D3DXMatrixTranslation(&matPosTrans, 50.f, 150.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matPosTrans);

	swprintf_s(m_szPos, L"X: %f  Y: %f", m_tInfo.vPos.x, m_tInfo.vPos.y);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPos, lstrlen(m_szPos), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Hp 출력
	D3DXMatrixTranslation(&matHpTrans, 600.f, 100.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matHpTrans);

	swprintf_s(m_szHp, L"(B)HP: %d / %d", m_tUnit.m_iHp, m_tUnit.m_iMaxHp);
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

void CTechGuardian::Release_Object()
{
	//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CTechGuardian::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	// Ball Attack: 11 ~ 15 반복
	if (m_eCurScene == BALL_ATTACK && m_bBallAttack && m_tFrame.dFrameStart >= 15.0)
	{
		m_tFrame.dFrameStart = 11.0;
	}

	else if (m_eCurScene == DASH && m_iDashCnt <= 5 && m_tFrame.dFrameStart >= 29.0)
	{
		m_tFrame.dFrameStart = 5.0;
	}

	// -----------------------------------------------------------------------------------------------

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_bInitFrame = false;

		if (m_eCurScene == WAKEUP)
		{
			//CSoundMgr::Get_Instance()->PlayBGM(L"Tech.wav");

			//CSoundMgr::Get_Instance()->PlayBGM(L"TechBGM.wav");
			m_bIntro = true;
			m_eCurScene = IDLE;
		}

		else if (m_eCurScene == TELEPORT || m_eCurScene == DASH || m_eCurScene == ELECTRIC_PART)
		{
			m_eCurScene = IDLE;
		}

		else if (m_eCurScene == IDLE)
		{
			// Electric Part 제거
			if (!m_listPart.empty())
			{
				for (auto& pPart : m_listPart)
					pPart->Set_Dead();
				m_listPart.clear();
			}
		}

		else if (m_eCurScene == DEATH)
			m_bDead = true;

		m_tFrame.dFrameStart = 0.0;
	}
}

void CTechGuardian::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CTechGuardian::WAKEUP:
			m_wstrStateKey = L"BOSS_D_WAKEUP";
			m_tFrame = { 0.0, 121.0 };
			m_dFrameSpeed = 0.05;
			break;
		case CTechGuardian::IDLE:
			m_wstrStateKey = L"BOSS_D_IDLE";
			m_tFrame = { 0.0, 24.0 };
			m_dFrameSpeed = 1.0;
			break;
		case CTechGuardian::TELEPORT:
			m_wstrStateKey = L"BOSS_D_TELEPORT";
			m_tFrame = { 0.0, 34.0 };
			m_dFrameSpeed = 1.0;
			break;
		case CTechGuardian::BALL_ATTACK:
			m_wstrStateKey = L"BOSS_D_BODY_BALL";
			m_tFrame = { 0.0, 25.0 };
			m_dFrameSpeed = 1.0;
			break;
		case CTechGuardian::ELECTRIC_PART:
			m_wstrStateKey = L"BOSS_D_ELECTIC";
			m_tFrame = { 0.0, 18.0 };
			m_dFrameSpeed = 1.0;
			break;
		case CTechGuardian::DASH:
			m_wstrStateKey = L"BOSS_D_BODY_DASH";
			m_tFrame = { 0.0, 34.0 };
			m_dFrameSpeed = 0.5;
			break;
		case CTechGuardian::DEATH:
			m_wstrStateKey = L"BOSS_D_DEATH";
			m_tFrame = { 0.0, 106.0 };
			m_dFrameSpeed = 0.15;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CTechGuardian::Initialize()
{
	m_bTeleport = false;
	m_bAttack = false;
	m_bInitFrame = false;

	// Ball Attack
	m_fAngle = 0.f;
	m_bBallAttack = false;
	m_bCreateBalls = false;

	// Dash
	m_bSetTarget = false;
	m_bDash = true;
	m_bDashEffect = false;
	m_iDashCnt = 0;

	// Electric Part
	m_fPartAngle = 0.f;
	m_bCreateCircle = false;
	m_bCreatePart = false;
	m_fPartDis = 30.f;
	m_dPartTime = 0.0;
}

void CTechGuardian::Progress()
{
	// Teleport = false !!!

	//m_dWaveTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dWaveTime >= 2.0 && m_dWaveTime < 7.0)
	{
		//if (!m_bTeleport)
		//	Teleport(1200.f, 1000.f);

		Ball_Attack();
	}
}

void CTechGuardian::Teleport(float fX, float fY)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);

	m_eCurScene = TELEPORT;
	CSoundMgr::Get_Instance()->PlaySound(L"Tech_Telpo.wav", CSoundMgr::EFFECT);

	if (m_eCurScene == TELEPORT && m_tFrame.dFrameStart >= 13.0)
	{
		m_tInfo.vPos = { fX, fY, 0.f };
		m_bTeleport = true;
	}
}

void CTechGuardian::Ball_Attack()
{
	m_eCurScene = BALL_ATTACK;
	m_bBallAttack = true;
	CSoundMgr::Get_Instance()->PlaySound(L"Tech_Ball.wav", CSoundMgr::EFFECT);

	// 11 ~ 15 -> 회전
	//	m_fAngle += 1.f;

	if (!m_bCreateBalls && m_tFrame.dFrameStart >= 11.0)
	{
		// 몸 속 전기
		m_pBodyBeam = CAbstractFactory<CBodyBeam>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, m_pBodyBeam);

		// 몸 밖 전기
		m_pOutBeam = CAbstractFactory<COutBeam>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BEAM, m_pOutBeam);

		// 빔
		m_pBeam = CAbstractFactory<CBeam>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, m_pBeam);

		m_bCreateBalls = true;
	}
}

void CTechGuardian::Dash()
{
	m_eCurScene = DASH;

	// 0 ~ 4: 준비 (처음 한번만)

	// 5 ~ 9: 대쉬 준비

	// 10 ~ 16: 대쉬 앞으로
	if (m_tFrame.dFrameStart >= 10.0 && m_tFrame.dFrameStart < 16.0)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Tech_Dash.wav", CSoundMgr::EFFECT);

		if (!m_bSetTarget)
		{
			m_pTarget = CObjMgr::Get_Instance()->Get_Player();

			if (!m_bDashEffect)
			{
				CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
				CObjMgr::Get_Instance()->Add_Object(OBJ::EFFECT, pObj);
				dynamic_cast<CEffect*>(pObj)->Set_EffectID(EFFECT::DASH);

				m_bDashEffect = true;

			}
			m_tInfo.vDir = m_pTarget->Get_Info().vPos - m_tInfo.vPos;

			++m_iDashCnt;
			m_bSetTarget = true;
		}

		m_tInfo.vPos += m_tInfo.vDir * 0.04f;
	}


	// 17 ~ 29: 다음 대쉬 대기
	else if (m_tFrame.dFrameStart > 16.0 && m_tFrame.dFrameStart < 29.0)
	{
		m_bSetTarget = false;
		m_bDashEffect = false;

		if (m_iDashCnt > 5)
		{
			m_bAttack = true;
		}
	}
}

void CTechGuardian::Electric_Part(int iDirNum)
{
	m_eCurScene = ELECTRIC_PART;

	if (!m_bInitFrame)
	{
		m_tFrame.dFrameStart = 0.0;
		m_bInitFrame = true;
	}

	// Circle Part 생성
	if (!m_bCreateCircle)
	{
		for (; m_fPartAngle <= 360.f; m_fPartAngle += 5.f)
		{
			float fX = m_tInfo.vPos.x + (cosf(D3DXToRadian(m_fPartAngle)) * 180.f);
			float fY = m_tInfo.vPos.y - (sinf(D3DXToRadian(m_fPartAngle)) * 180.f);

			CObj* pObj = CAbstractFactory<CElectricPart>::Create(fX, fY);
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
			m_listPart.emplace_back(pObj);
		}

		m_bCreateCircle = true;
	}

	// Circle Part 삭제 & 바닥 Part 생성
	if (m_bCreateCircle && m_tFrame.dFrameStart >= 15.0)
	{
		for (auto& pPart : m_listPart)
			pPart->Set_Dead();
		m_listPart.clear();

		switch (iDirNum)
		{
			case 1:		// LEFT (600, 750)
			{
				for (int i = 0; i < 230; ++i)
				{
					float fY = (float)((rand() % (-400)) + (m_tInfo.vPos.y - 200.f));

					m_fPartAngle = (float)(rand() % 360);

					CObj* pObj = CAbstractFactory<CElectricPart>::Create(m_tInfo.vPos.x + 150.f + m_fPartDis, fY, m_fPartAngle);
					CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
					m_listPart.emplace_back(pObj);

					m_fPartDis += 5.f;
				}
			}
			break;

			case 2:		// RIGHT (1800, 750)
			{
				for (int i = 0; i < 230; ++i)
				{
					float fY = (float)((rand() % (-400)) + (m_tInfo.vPos.y - 200.f));

					m_fPartAngle = (float)(rand() % 360);

					CObj* pObj = CAbstractFactory<CElectricPart>::Create(m_tInfo.vPos.x - 150.f - m_fPartDis, fY, m_fPartAngle);
					CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
					m_listPart.emplace_back(pObj);

					m_fPartDis += 5.f;
				}
			}
			break;

			case 3:		// UP (1200, 1150)
			{
				for (int i = 0; i < 150; ++i)
				{
					float fX = (float)((rand() % (-400)) + (m_tInfo.vPos.x - 200.f));

					m_fPartAngle = (float)(rand() % 360);

					CObj* pObj = CAbstractFactory<CElectricPart>::Create(fX, m_tInfo.vPos.y - 150.f - m_fPartDis, m_fPartAngle);
					CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
					m_listPart.emplace_back(pObj);

					m_fPartDis += 5.f;
				}
			}
			break;

			case 4:		// DOWN (1200, 350)
			{
				for (int i = 0; i < 150; ++i)
				{
					float fX = (float)((rand() % (-400)) + (m_tInfo.vPos.x - 200.f));

					m_fPartAngle = (float)(rand() % 360);

					CObj* pObj = CAbstractFactory<CElectricPart>::Create(fX, m_tInfo.vPos.y + 150.f + m_fPartDis, m_fPartAngle);
					CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
					m_listPart.emplace_back(pObj);

					m_fPartDis += 5.f;

				}
			}
			break;
		}

		m_bAttack = true;
	}

	else
	{
		m_dPartTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dPartTime >= 10.0)
		{
			m_fPartAngle = 0.f;

			for (auto& pPart : m_listPart)
				pPart->Set_Dead();
			m_listPart.clear();

		//	m_dPartTime = 0.0;
		}
	}
}