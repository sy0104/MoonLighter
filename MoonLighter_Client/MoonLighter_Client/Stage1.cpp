#include "stdafx.h"
#include "Stage1.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Graphic_Device.h"
#include "Player.h"
#include "GolemKing.h"
#include "PlayerHpBar.h"
#include "PlayerHp.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

#include "TimeMgr.h"

CStage1::CStage1()
	: m_pPlayer(nullptr), m_pGolemKing(nullptr), m_dSceneChangeTime(0.0)
{
}


CStage1::~CStage1()
{
	Release_Scene();
}

void CStage1::Ready_Scene()
{
	// Player
	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PLAYER, m_pPlayer);

	// Boss - Golem King
	m_pGolemKing = CAbstractFactory<CGolemKing>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::BOSS, m_pGolemKing);
}

void CStage1::Update_Scene()
{
	if (GetAsyncKeyState(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);
	}

	CObjMgr::Get_Instance()->Update_ObjMgr();


	// 충돌체크
	if (!CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).empty())
	{
		CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, m_pGolemKing);
		//CCollisionMgr::Collision_Player_To_Rock(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::ROCK));
		CCollisionMgr::Collision_Player_To_Rock(CObjMgr::Get_Instance()->Get_List(OBJ::ROCK), m_pPlayer);
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK));
		CCollisionMgr::Collision_Weapon_To_Boss(m_pPlayer, m_pGolemKing);
		CCollisionMgr::Collision_Arrow_To_Boss(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pGolemKing);
	}
	CCollisionMgr::Collision_Player_To_Item(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::ITEM));


	if (CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).empty())
	{
		m_dSceneChangeTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dSceneChangeTime >= 8.0)
			CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);

		for (auto& pMonster : CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER))
			pMonster->Set_Dead();

		for (auto& pRock : CObjMgr::Get_Instance()->Get_List(OBJ::ROCK))
			pRock->Set_Dead();

		for (auto& pMonsterBack : CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK))
			pMonsterBack->Set_Dead();

		for (auto& pShadow : CObjMgr::Get_Instance()->Get_List(OBJ::SHADOW))
			pShadow->Set_Dead();
	}
}

void CStage1::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CStage1::Render_Scene()
{
	D3DXMATRIX	matScale, matTrans, matWorld;
	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BACKGROUND", L"BossRoom", 0);
	assert(!(pTexInfo == nullptr));

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	CObjMgr::Get_Instance()->Render_ObjMgr();
}

void CStage1::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::ROCK);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::SHADOW);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER_BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::ITEM);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
