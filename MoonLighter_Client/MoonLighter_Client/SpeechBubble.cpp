#include "stdafx.h"
#include "SpeechBubble.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CSpeechBubble::CSpeechBubble()
{
}


CSpeechBubble::~CSpeechBubble()
{
	Release_Object();
}

HRESULT CSpeechBubble::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/SpeechBubble.png", L"SpeechBubble")))
	{
		ERR_MSG(L"SpeechBubble Insert Failed");
	}



	return S_OK;
}

int CSpeechBubble::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;




	return OBJ_NOEVENT;
}

void CSpeechBubble::LateUpdate_Object()
{
}

void CSpeechBubble::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"SpeechBubble");

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CSpeechBubble::Release_Object()
{
}
