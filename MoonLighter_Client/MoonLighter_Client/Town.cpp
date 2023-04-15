#include "stdafx.h"
#include "Town.h"
#include "ObjMgr.h"
#include "TownPlayer.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "SceneMgr.h"
#include "Npc.h"
#include "SoundMgr.h"


CTown::CTown()
	: m_pPlayer(nullptr), m_pNathan(nullptr), m_pSakura(nullptr), m_pWorker(nullptr), m_bBgm(false)
{
}


CTown::~CTown()
{
	Release_Scene();
}

void CTown::Ready_Scene()
{
	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"Golem.wav");
//	CSoundMgr::Get_Instance()->PlayBGM(L"ShopBGM.wav");


	// Player
	m_pPlayer = CAbstractFactory<CTownPlayer>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::TOWN_PLAYER, m_pPlayer);

	// Npc
	m_pNathan = CAbstractFactory<CNpc>::Create(950.f, 350.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, m_pNathan);
	dynamic_cast<CNpc*>(m_pNathan)->Set_ObjectKey(L"Nathan");
	dynamic_cast<CNpc*>(m_pNathan)->Set_StateKey(L"WALK_DOWN");
	dynamic_cast<CNpc*>(m_pNathan)->Set_Frame(8.0);
	dynamic_cast<CNpc*>(m_pNathan)->Set_Walk();

	m_pSakura = CAbstractFactory<CNpc>::Create(400.f, 245.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, m_pSakura);
	dynamic_cast<CNpc*>(m_pSakura)->Set_ObjectKey(L"Sakura");
	dynamic_cast<CNpc*>(m_pSakura)->Set_StateKey(L"WALK_LEFT");
	dynamic_cast<CNpc*>(m_pSakura)->Set_Frame(8.0);
	dynamic_cast<CNpc*>(m_pSakura)->Set_Walk();

	m_pWorker = CAbstractFactory<CNpc>::Create(145.f, 600.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::NPC, m_pWorker);
	dynamic_cast<CNpc*>(m_pWorker)->Set_ObjectKey(L"Worker");
	dynamic_cast<CNpc*>(m_pWorker)->Set_StateKey(L"WALK_UP");
	dynamic_cast<CNpc*>(m_pWorker)->Set_Frame(8.0);
	dynamic_cast<CNpc*>(m_pWorker)->Set_Walk();
}

void CTown::Update_Scene()
{
	if (GetAsyncKeyState('Z'))
	{
		if (m_pPlayer->Get_Info().vPos.x >= 600.f)
			CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::SHOP);
		else
			CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);
	}

	CObjMgr::Get_Instance()->Update_ObjMgr();
}

void CTown::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CTown::Render_Scene()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BACKGROUND", L"Town", 0);
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

void CTown::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::TOWN_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::NPC);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);

}
