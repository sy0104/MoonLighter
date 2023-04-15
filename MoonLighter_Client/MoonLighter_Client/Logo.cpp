#include "stdafx.h"
#include "Logo.h"
#include "Player.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release_Scene();
}

void CLogo::Ready_Scene()
{
	m_tFrame = { 0.0, 15.0 };

	// Logo
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/GameLogo_0.png", L"GameLogo")))
	{
		ERR_MSG(L"GameLogo Insert Failed");
	}

	// Door_Left
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/GameLogo_1.png", L"Door_Left")))
	{
		ERR_MSG(L"Door_Left Insert Failed");
	}

	// Door_Right
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/GameLogo_2.png", L"Door_Right")))
	{
		ERR_MSG(L"Door_Right Insert Failed");
	}


	// Sound
	CSoundMgr::Get_Instance()->PlayBGM(L"MainBGM.wav");
}

void CLogo::Update_Scene()
{
	if (GetAsyncKeyState(VK_RETURN))
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::MAP);

	Frame_Move();
}

void CLogo::LateUpdate_Scene()
{
}

void CLogo::Render_Scene()
{
	// Background
	D3DXMATRIX	 matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"LOGO", L"LogoScreen", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixTranslation(&matTrans, 600.f, 400.f, 0.f);

	matWorld = matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------

	// Moonlighter Logo
	D3DXMATRIX	matTrans2;

	const TEXINFO* pTexInfo2 = CTextureMgr::Get_Instance()->Get_TexInfo(L"GameLogo");

	assert(!(pTexInfo2 == nullptr));

	float fCenterX2 = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY2 = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixTranslation(&matTrans2, 1000.f, 500.f, 0.f);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans2);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo2->pTexture, nullptr, &D3DXVECTOR3(fCenterX2, fCenterY2, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------

	// Door_Left
	D3DXMATRIX	matTrans3;

	const TEXINFO* pTexInfo3 = CTextureMgr::Get_Instance()->Get_TexInfo(L"Door_Left");

	assert(!(pTexInfo3 == nullptr));

	float fCenterX3 = float(pTexInfo3->tTexInfo.Width >> 1);
	float fCenterY3 = float(pTexInfo3->tTexInfo.Height >> 1);

	D3DXMatrixTranslation(&matTrans3, -200.f, 400.f, 0.f);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans3);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo3->pTexture, nullptr, &D3DXVECTOR3(fCenterX3, fCenterY3, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	// ----------------------------------------------------------------------------------------------------------------------------------------------------

	// Door_Right
	D3DXMATRIX	matTrans4;

	const TEXINFO* pTexInfo4 = CTextureMgr::Get_Instance()->Get_TexInfo(L"Door_Right");

	assert(!(pTexInfo4 == nullptr));

	float fCenterX4 = float(pTexInfo4->tTexInfo.Width >> 1);
	float fCenterY4 = float(pTexInfo4->tTexInfo.Height >> 1);

	D3DXMatrixTranslation(&matTrans4, 1400.f, 400.f, 0.f);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans4);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo4->pTexture, nullptr, &D3DXVECTOR3(fCenterX4, fCenterY4, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CLogo::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

void CLogo::Frame_Move()
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd* CTimeMgr::Get_Instance()->Get_DeltaTime() * 1.0;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
		m_tFrame.dFrameStart = 0.0;
}
