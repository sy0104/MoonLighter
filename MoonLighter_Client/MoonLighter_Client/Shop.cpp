#include "stdafx.h"
#include "Shop.h"
#include "ObjMgr.h"
#include "TownPlayer.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "SceneMgr.h"
#include "ShopTable.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

#include "Showcase.h"
#include "Inven.h"

CShop::CShop()
	: m_pPlayer(nullptr), m_pTable1(nullptr), m_pTable2(nullptr), m_pTable3(nullptr), m_pTable4(nullptr)
	, m_bBgm(false)
{

}

CShop::~CShop()
{
	Release_Scene();
}

void CShop::Ready_Scene()
{
	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"Golem.wav");
	//CSoundMgr::Get_Instance()->PlayBGM(L"ShopBGM.wav");

	// Player
	m_pPlayer = CAbstractFactory<CTownPlayer>::Create(670.f, 445.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::TOWN_PLAYER, m_pPlayer);

	// Table
	m_pTable1 = CAbstractFactory<CShopTable>::Create(390.f, 240.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FURNITURE, m_pTable1);
	dynamic_cast<CInven*>(dynamic_cast<CTownPlayer*>(m_pPlayer)->Get_Inven())->Insert_Table(m_pTable1);

	m_pTable2 = CAbstractFactory<CShopTable>::Create(460.f, 240.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FURNITURE, m_pTable2);
	dynamic_cast<CInven*>(dynamic_cast<CTownPlayer*>(m_pPlayer)->Get_Inven())->Insert_Table(m_pTable2);

	m_pTable3 = CAbstractFactory<CShopTable>::Create(390.f, 320.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FURNITURE, m_pTable3);
	dynamic_cast<CInven*>(dynamic_cast<CTownPlayer*>(m_pPlayer)->Get_Inven())->Insert_Table(m_pTable3);

	m_pTable4 = CAbstractFactory<CShopTable>::Create(460.f, 320.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FURNITURE, m_pTable4);
	dynamic_cast<CInven*>(dynamic_cast<CTownPlayer*>(m_pPlayer)->Get_Inven())->Insert_Table(m_pTable4);

}

void CShop::Update_Scene()
{
	if (GetAsyncKeyState('T'))
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::TOWN);


	CObjMgr::Get_Instance()->Update_ObjMgr();

	// Ãæµ¹
	if (m_pPlayer && !CObjMgr::Get_Instance()->Get_List(OBJ::FURNITURE).empty())
		CCollisionMgr::Collisoin_Player_To_Table(m_pPlayer, CObjMgr::Get_Instance()->Get_List(OBJ::FURNITURE));
} 

void CShop::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CShop::Render_Scene()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BACKGROUND", L"Town", 1);
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

void CShop::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::TOWN_PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::NPC);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::FURNITURE);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);

}
