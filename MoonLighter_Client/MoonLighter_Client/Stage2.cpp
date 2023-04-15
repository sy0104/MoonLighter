#include "stdafx.h"
#include "Stage2.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "Player.h"
#include "Plant.h"
#include "PlayerHpBar.h"
#include "PlayerHp.h"
#include "SoundMgr.h"

#include "TimeMgr.h"


CStage2::CStage2()
	: m_pPlayer(nullptr), m_pPlant(nullptr), m_dSceneChangeTime(0.0)
{
}


CStage2::~CStage2()
{
	Release_Scene();
}

void CStage2::Ready_Scene()
{
	// Sound
	//CSoundMgr::Get_Instance()->PlayBGM(L"PlantBGM.wav");

	// Player
	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PLAYER, m_pPlayer);

	m_pPlant = CAbstractFactory<CPlant>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::BACK, m_pPlant);

}

void CStage2::Update_Scene()
{

	if (GetAsyncKeyState(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);
	}

	CObjMgr::Get_Instance()->Update_ObjMgr();

	// 충돌체크
	if (!CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).empty())
	{
		if (dynamic_cast<CPlant*>(m_pPlant)->Get_Created())
		{
			CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, dynamic_cast<CPlant*>(m_pPlant)->Get_PlantHead());
			CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, dynamic_cast<CPlant*>(m_pPlant)->Get_PlantBody());
			CCollisionMgr::Collision_Weapon_To_Boss(m_pPlayer, dynamic_cast<CPlant*>(m_pPlant)->Get_PlantHead());
			CCollisionMgr::Collision_Weapon_To_Boss(m_pPlayer, dynamic_cast<CPlant*>(m_pPlant)->Get_PlantBody());
			CCollisionMgr::Collision_Arrow_To_Boss(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), dynamic_cast<CPlant*>(m_pPlant)->Get_PlantHead());
			CCollisionMgr::Collision_Arrow_To_Boss(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), dynamic_cast<CPlant*>(m_pPlant)->Get_PlantBody());
		}
		CCollisionMgr::Collision_Weapon_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::BULLET));
		CCollisionMgr::Collision_Arrow_To_Monster(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
	}

	CCollisionMgr::Collision_Player_To_Item(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::ITEM));


	if (CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).empty())
	{
		m_dSceneChangeTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dSceneChangeTime >= 8.0)
			CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);

		for (auto& pMonster : CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER))
			pMonster->Set_Dead();
		
		for (auto& pMonsterBack : CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK))
			pMonsterBack->Set_Dead();

		for (auto& pBullet : CObjMgr::Get_Instance()->Get_List(OBJ::BULLET))
			pBullet->Set_Dead();
	}
}

void CStage2::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CStage2::Render_Scene()
{
	D3DXMATRIX	matScale, matTrans, matWorld;
	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BACKGROUND", L"BossRoom", 1);
	assert(!(pTexInfo == nullptr));

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CObjMgr::Get_Instance()->Render_ObjMgr();
}

void CStage2::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER_BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BULLET);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::ITEM);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
