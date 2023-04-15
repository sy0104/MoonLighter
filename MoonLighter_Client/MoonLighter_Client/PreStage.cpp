#include "stdafx.h"
#include "PreStage.h"
#include "Player.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "Chest.h"
#include "Pillar.h"
#include "Slime.h"
#include "TimeMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"
#include "StagePotal.h"
#include "Arrow.h"

CPreStage::CPreStage()
	: m_pPlayer(nullptr), m_pPotal(nullptr), m_pChest(nullptr), m_pPillar1(nullptr), m_pPillar2(nullptr), m_pPillar3(nullptr), m_pPillar4(nullptr)
	, m_bPillar1(false), m_bPillar2(false), m_bPillar3(false), m_bPillar4(false), m_dPillarTime(0.0), m_bOpen(false), m_bPotal(false)
{
}


CPreStage::~CPreStage()
{
	Release_Scene();
}

void CPreStage::Ready_Scene()
{
	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"StageBGM.wav");

	// 배경 이미지
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Golem_Stage.png", L"GolemStage")))
	{
		ERR_MSG(L"GolemStage Insert Failed");
	}

	// Player
	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PLAYER, m_pPlayer);

	// Potal
	m_pPotal = CAbstractFactory<CStagePotal>::Create(600.f, 90.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::POTAL, m_pPotal);

	// Chest
	m_pChest = CAbstractFactory<CChest>::Create(300.f, 250.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::CHEST, m_pChest);

	// Pillar
	m_pPillar1 = CAbstractFactory<CPillar>::Create(400.f, 350.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::PILLAR, m_pPillar1);

	m_pPillar2 = CAbstractFactory<CPillar>::Create(400.f, 150.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::PILLAR, m_pPillar2);

	m_pPillar3 = CAbstractFactory<CPillar>::Create(200.f, 150.f);
	dynamic_cast<CPillar*>(m_pPillar3)->Set_ExtendY();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PILLAR, m_pPillar3);

	m_pPillar4 = CAbstractFactory<CPillar>::Create(200.f, 350.f);
	dynamic_cast<CPillar*>(m_pPillar4)->Set_ExtendX();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PILLAR, m_pPillar4);

	// Monster
	CObj* pObj = CAbstractFactory<CSlime>::Create(800.f, 400.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);

	pObj = CAbstractFactory<CSlime>::Create(1000.f, 600.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, pObj);
}

void CPreStage::Update_Scene()
{
	if (GetAsyncKeyState(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);
	}

	if (!CObjMgr::Get_Instance()->Get_List(OBJ::ARROW).empty())
	{
		if (m_pPillar1)
			Collision_Arrow_To_Pillar(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pPillar1, 1);
		if (m_pPillar2)
			Collision_Arrow_To_Pillar(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pPillar2, 2);
		if (m_pPillar3)
			Collision_Arrow_To_Pillar(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pPillar3, 3);
		if (m_pPillar4)
			Collision_Arrow_To_Pillar(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pPillar4, 4);
	}

	// 시간 제한
	if (m_bPillar1)
	{
		m_dPillarTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_bPillar1 && m_bPillar2 && m_bPillar3 && m_bPillar4)
		{
			if (!m_bOpen)
			{
				dynamic_cast<CChest*>(m_pChest)->Set_Open();
				m_bOpen = true;
			}
			//dynamic_cast<CChest*>(m_pChest)->Set_StateKey(L"OPEN");
			//dynamic_cast<CChest*>(m_pChest)->Set_Frame(5.0);
		}

		else if (m_dPillarTime >= 3.0)
		{
			
			dynamic_cast<CPillar*>(m_pPillar1)->Set_CheckFalse();
			dynamic_cast<CPillar*>(m_pPillar2)->Set_CheckFalse();
			dynamic_cast<CPillar*>(m_pPillar3)->Set_CheckFalse();
			dynamic_cast<CPillar*>(m_pPillar4)->Set_CheckFalse();

			m_bPillar1 = false;
			m_bPillar2 = false;
			m_bPillar3 = false;
			m_bPillar4 = false;

			m_dPillarTime = 0.0;
		}
	}



	if (m_bPillar1 && m_bPillar2 && m_bPillar3 && m_bPillar4)
	{
		m_pPillar1->Set_Dead();
		m_pPillar2->Set_Dead();
		m_pPillar3->Set_Dead();
		m_pPillar4->Set_Dead();
	}
	
	if (!CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER).empty())
	{
		CCollisionMgr::Collision_Weapon_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Arrow_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
	}

	else
	{
		if (!m_bPotal)
		{
			dynamic_cast<CStagePotal*>(m_pPotal)->Set_Open();
			m_bPotal = true;
		}
	}


	CCollisionMgr::Collision_Player_To_Item(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::ITEM));

	CObjMgr::Get_Instance()->Update_ObjMgr();
}

void CPreStage::LateUpdate_Scene()
{

	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CPreStage::Render_Scene()
{
	// 배경
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"GolemStage");
	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CObjMgr::Get_Instance()->Render_ObjMgr();
}

void CPreStage::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::ARROW);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::POTAL);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PILLAR);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::CHEST);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CPreStage::Collision_Arrow_To_Pillar(list<CObj*>& _pArrow, CObj* pPillar, int iNum)
{
	RECT rc = {};

	for (auto& Arrow : _pArrow)
	{
		if (IntersectRect(&rc, &(Arrow->Get_Rect()), &(pPillar->Get_Rect())))
		{
			if (iNum == 2)
			{
				dynamic_cast<CArrow*>(Arrow)->Set_StateKey(L"LEFT");
				dynamic_cast<CArrow*>(Arrow)->Set_Dir(DIR::LEFT);
			}
			else if (iNum == 3)
			{
				dynamic_cast<CArrow*>(Arrow)->Set_StateKey(L"DOWN");
				dynamic_cast<CArrow*>(Arrow)->Set_Dir(DIR::DOWN);
			}

			dynamic_cast<CPillar*>(pPillar)->Set_CheckTrue();
			if (iNum == 1)
				m_bPillar1 = true;
			else if (iNum == 2)
				m_bPillar2 = true;
			else if (iNum == 3)
				m_bPillar3 = true;
			else if (iNum == 4)
				m_bPillar4 = true;
		}
	}
}
