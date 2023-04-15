#include "stdafx.h"
#include "Stage3.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "TechGuardian.h"
#include "ElectricRoom.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "TimeMgr.h"

CStage3::CStage3()
	: m_pPlayer(nullptr), m_pTechGuardian(nullptr), m_dSceneChangeTime(0.0)
{
}


CStage3::~CStage3()
{
	Release_Scene();
}

void CStage3::Ready_Scene()
{
	// Player
	m_pPlayer = CAbstractFactory<CPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::PLAYER, m_pPlayer);

	// Boss
	m_pTechGuardian = CAbstractFactory<CTechGuardian>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::BOSS, m_pTechGuardian);

	// Electric Room
	CObj* pObj = CAbstractFactory<CElectricRoom>::Create(1200.f, 270.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, pObj);

	pObj = CAbstractFactory<CElectricRoom>::Create(1200.f, 1275.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, pObj);

	CSoundMgr::Get_Instance()->PlayBGM(L"Golem3.wav");

	//CSoundMgr::Get_Instance()->PlayBGM(L"TechBGM.wav");
}

void CStage3::Update_Scene()
{
	if (GetAsyncKeyState(VK_BACK))
	{
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);
	}

	CObjMgr::Get_Instance()->Update_ObjMgr();

	// 충돌체크
	if (!CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).empty())
	{
		CCollisionMgr::Collision_Player_To_Boss(m_pPlayer, m_pTechGuardian);
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK));
		CCollisionMgr::Collision_Player_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::BEAM));

		CCollisionMgr::Collision_Weapon_To_Boss(m_pPlayer, m_pTechGuardian);
		CCollisionMgr::Collision_Weapon_To_Monster(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER));
		CCollisionMgr::Collision_Arrow_To_Boss(CObjMgr::Get_Instance()->Get_List(OBJ::ARROW), m_pTechGuardian);
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

		for (auto& pBeam : CObjMgr::Get_Instance()->Get_List(OBJ::BEAM))
			pBeam->Set_Dead();

		for (auto& pMonsterBack : CObjMgr::Get_Instance()->Get_List(OBJ::MONSTER_BACK))
			pMonsterBack->Set_Dead();
	}
}

void CStage3::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CStage3::Render_Scene()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	CScrollMgr::Get_Instance()->Set_ScrollLockX(-1200.f);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(-750.f);

	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BACKGROUND", L"BossRoom", 3);
	assert(!(pTexInfo == nullptr));

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CObjMgr::Get_Instance()->Render_ObjMgr();

}

void CStage3::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BOSS);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::MONSTER_BACK);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::BEAM);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::ITEM);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
