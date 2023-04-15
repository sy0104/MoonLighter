#include "stdafx.h"
#include "PriceDown.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CPriceDown::CPriceDown()
{
}


CPriceDown::~CPriceDown()
{
	Release_Object();
}

HRESULT CPriceDown::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.5f, 2.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Price_Down.png", L"PriceDown")))
	{
		ERR_MSG(L"PriceDown Insert Failed");
	}

	m_iAlpha = 0;


	return S_OK;
}

int CPriceDown::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CPriceDown::LateUpdate_Object()
{
}

void CPriceDown::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"PriceDown");

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 1.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 1.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 1.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 1.5f));

//
//#ifdef _DEBUG
//	// 충돌박스 출력
//	D3DXVECTOR2 vLine[5] =
//	{
//		{ (float)m_tRect.left, (float)m_tRect.top },
//		{ (float)m_tRect.right, (float)m_tRect.top },
//		{ (float)m_tRect.right, (float)m_tRect.bottom },
//		{ (float)m_tRect.left, (float)m_tRect.bottom },
//		{ (float)m_tRect.left, (float)m_tRect.top }
//	};
//
//
//	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
//	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
//	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
//#endif

}

void CPriceDown::Release_Object()
{
}
