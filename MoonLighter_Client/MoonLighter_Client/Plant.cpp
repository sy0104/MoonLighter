#include "stdafx.h"
#include "Plant.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "PlantHead.h"
#include "PlantBody.h"
#include "PlantExtra.h"

#include "SoundMgr.h"

CPlant::CPlant()
	: m_ePreScene(END), m_eCurScene(END), m_wstrObjectKey(L""), m_wstrStateKey(L"")
	, m_pTarget(nullptr), m_pPlantExtra(nullptr), m_pPlantHead(nullptr), m_pPlantBody(nullptr), m_dWaveTime(0.0)
	, m_bSetShake(false), m_bJump(false), m_iJumpCnt(0), m_bPots(false), m_bCreatePlants(false), m_bCreaetExtra(false), m_bSetDead(false)
{
}


CPlant::~CPlant()
{
	Release_Object();
}

HRESULT CPlant::Ready_Object()
{
	m_tInfo.vPos = { 900.f, 600.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };


	m_ePreScene = IDLE;
	m_eCurScene = IDLE;

	m_pPlantExtra = CAbstractFactory<CPlantExtra>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
	CObjMgr::Get_Instance()->Add_Object(OBJ::BOSS, m_pPlantExtra);
	dynamic_cast<CPlantExtra*>(m_pPlantExtra)->Set_StateKey(L"WAKEUP");
	dynamic_cast<CPlantExtra*>(m_pPlantExtra)->Set_Frame(168.0);

	return S_OK;
}

int CPlant::Update_Object()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (!m_bCreatePlants && m_pPlantExtra->Get_Dead())
	{
		m_pPlantBody = CAbstractFactory<CPlantBody>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BOSS, m_pPlantBody);
		dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_IDLE);

		m_pPlantHead = CAbstractFactory<CPlantHead>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 10.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BOSS, m_pPlantHead);
		dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_IDLE);

		m_bCreatePlants = true;
	}


	if (m_pPlantBody && m_pPlantHead)
	{
		if (m_bJump && m_dWaveTime <= 24.0)
			CSoundMgr::Get_Instance()->PlaySound(L"Plant_Jump.wav", CSoundMgr::EFFECT);


		if (m_pPlantHead->Get_UnitInfo().m_iHp <= 0 && m_pPlantBody->Get_UnitInfo().m_iHp <= 0 && !m_bSetDead)
		{
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_DeathMotion();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_DeathMotion();
			m_bSetDead = true;
			//	m_bDead = true;
		}

		else if (m_pPlantHead->Get_UnitInfo().m_iHp > 0 && m_pPlantBody->Get_UnitInfo().m_iHp > 0)
		{
			Progress();
			Scene_Change();
		}
	}

	else if (m_bCreatePlants && !m_pPlantBody && !m_pPlantHead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		m_bDead = true;
	}
	


	return OBJ_NOEVENT;
}

void CPlant::LateUpdate_Object()
{

}

void CPlant::Render_Object()
{

#ifdef _DEBUG
	// Wave 시간 출력
	D3DXMATRIX	matFontTrans;

	D3DXMatrixTranslation(&matFontTrans, 1000.f, 100.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matFontTrans);

	swprintf_s(m_szWaveTime, L"Time: %f", m_dWaveTime);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szWaveTime, lstrlen(m_szWaveTime), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

#endif
}

void CPlant::Release_Object()
{
}


void CPlant::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CPlant::WAKEUP:
			m_tFrame = { 0.0, 168.0 };
			break;
		case CPlant::IDLE:
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_IDLE);
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_IDLE);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Move();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Move();
			break;
		case CPlant::POTS:	// Particle
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_POTS);
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_IDLE);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Stop();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Stop();
			break;
		case CPlant::STEM:	// Root
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_IDLE);
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_STEM);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Stop();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Stop();
			break;
		case CPlant::JUMP:
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_JUMP);
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_JUMP);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Stop();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Stop();
			break;
		case CPlant::SHAKE:
			if (!m_bSetShake)
			{
				dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_STARTSHAKE);
				m_bSetShake = true;
			}
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_IDLE);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Move();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Move();
			break;
		case CPlant::DEATH:
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_DEATH);
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Body(PLANT::BODY_DEATH);
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Stop();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_Stop();
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

void CPlant::Initialize()
{
	m_eCurScene = IDLE;

	// pots
	m_bPots = false;

	// stem
	dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_CreateRoot();
	dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_RootTime();

	// shake
	m_bSetShake = false;
	
	// jump
	m_bJump = false;
}

void CPlant::Progress()
{
	m_dWaveTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	// Wave1: Particle
	if (m_dWaveTime >= 3.0 && m_dWaveTime < 10.0)
	{
		if (!m_bPots && m_dWaveTime < 6.0)
		{
			m_eCurScene = POTS;
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Create_Particle();

			m_bPots = false;
		}
		else
		{
			m_eCurScene = IDLE;
		}
	}


	// Wave2: Root
	else if (m_dWaveTime >= 10.0 && m_dWaveTime < 20.0)
	{
		if (m_dWaveTime < 14.0)
		{
			m_eCurScene = STEM;

			if (m_pPlantBody->Get_Frame().dFrameStart > 18.0)
				dynamic_cast<CPlantBody*>(m_pPlantBody)->Create_Root();
		}

		else if (m_dWaveTime >= 14.0 && m_dWaveTime < 17.0)
		{
			m_eCurScene = IDLE;

			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_CreateRoot();
			dynamic_cast<CPlantBody*>(m_pPlantBody)->Set_RootTime();
		}

		else if (m_dWaveTime >= 17.0 && m_dWaveTime < 20.0)
		{
			m_eCurScene = STEM;

			if (m_pPlantBody->Get_Frame().dFrameStart > 18.0)
				dynamic_cast<CPlantBody*>(m_pPlantBody)->Create_Root();
		}
	}


	// Wave3: Jump
	else if (m_dWaveTime >= 20.0 && m_dWaveTime < 30.0)
	{
		if (!m_bJump && m_dWaveTime < 22.0)
		{
			m_eCurScene = JUMP;
			m_bJump = true;
		}

		else if (m_bJump && m_dWaveTime >= 22.0 && m_dWaveTime < 23.0)
		{
			m_eCurScene = IDLE;
			m_bJump = false;
		}

		else if (!m_bJump && m_dWaveTime >= 23.0 && m_dWaveTime < 25.0)
		{
			if (m_dWaveTime >= 24.0)
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
			m_eCurScene = JUMP;
			m_bJump = true;
		}

		else if (m_bJump && m_dWaveTime >= 25.0)
		{
			m_eCurScene = IDLE;
			m_bJump = false;
		}
	}


	// Wave4: Shake
	else if (m_dWaveTime >= 30.0 && m_dWaveTime < 40.0)
	{
		m_eCurScene = CPlant::SHAKE;

		if (m_dWaveTime >= 39.0)
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Set_Head(PLANT::HEAD_ENDSHAKE);

		if (m_dWaveTime >= 32.0 && m_dWaveTime < 36.0)
			dynamic_cast<CPlantHead*>(m_pPlantHead)->Create_Bullet();
	}


	else if (m_dWaveTime >= 40.0)
	{
		Initialize();
		m_dWaveTime = 0.0;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
	}

}
