#include "stdafx.h"
#include "MainMap.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Potal.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"


CMainMap::CMainMap()
	: m_pPlayer(nullptr)
{
}


CMainMap::~CMainMap()
{
	Release_Scene();
}

void CMainMap::Ready_Scene()
{
	// 배경 삽입
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Map/MainMap.png", L"MainMap")))
	{
		ERR_MSG(L"MainMap Insert Failed");
		return;
	}

	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"MapBGM.wav");

	// -----------------------------------------------------------------------------------------------

	// Player
	m_pPlayer = CAbstractFactory<CPlayer>::Create(907.f, 1070.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::PLAYER, m_pPlayer);

	// Potal
	CObj* pObj = CAbstractFactory<CPotal>::Create(460.f, 810.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::POTAL, pObj);
	dynamic_cast<CPotal*>(pObj)->Set_Key(L"POTAL_A", L"IDLE");
	dynamic_cast<CPotal*>(pObj)->Set_Frame(0.0, 6.0);

	pObj = CAbstractFactory<CPotal>::Create(1330.f, 810.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::POTAL, pObj);
	dynamic_cast<CPotal*>(pObj)->Set_Key(L"POTAL_B", L"IDLE");
	dynamic_cast<CPotal*>(pObj)->Set_Frame(0.0, 6.0);

	pObj = CAbstractFactory<CPotal>::Create(675.f, 520.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::POTAL, pObj);
	dynamic_cast<CPotal*>(pObj)->Set_Key(L"POTAL_A", L"IDLE");
	dynamic_cast<CPotal*>(pObj)->Set_Frame(0.0, 6.0);
}

void CMainMap::Update_Scene()
{
	if (CKeyMgr::Get_Instance()->Key_Down('T'))
	{
		if (m_pPlayer->Get_Info().vPos.y >= 1200.f)
			CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::TOWN);
	}

	CObjMgr::Get_Instance()->Update_ObjMgr();
}

void CMainMap::LateUpdate_Scene()
{
	CObjMgr::Get_Instance()->LateUpdate_ObjMgr();
}

void CMainMap::Render_Scene()
{
	// 배경
	D3DXMATRIX	matScale, matTrans, matWorld;
	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	CScrollMgr::Get_Instance()->Set_ScrollLockX(-600.f);
	CScrollMgr::Get_Instance()->Set_ScrollLockY(-400.f);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"MainMap");
	assert(!(pTexInfo == nullptr));

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CObjMgr::Get_Instance()->Render_ObjMgr();
}

void CMainMap::Release_Scene()
{
	CObjMgr::Get_Instance()->Delete_ID(OBJ::PLAYER);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::POTAL);
	CObjMgr::Get_Instance()->Delete_ID(OBJ::UI);

	CSoundMgr::Get_Instance()->StopAll();
}
