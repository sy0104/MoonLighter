#include "stdafx.h"
#include "ShopTable.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

#include "Item.h"

CShopTable::CShopTable()
	: m_pItem(nullptr), m_iItemAlpha(255)
{
}


CShopTable::~CShopTable()
{
	Release_Object();
}

HRESULT CShopTable::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.5f, 2.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Table.png", L"Table")))
	{
		ERR_MSG(L"Table Insert Failed");
	}

	return S_OK;
}

int CShopTable::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;



	return OBJ_NOEVENT;
}

void CShopTable::LateUpdate_Object()
{
}

void CShopTable::Render_Object()
{
	// Shop Table
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Table");

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

	// Item
	if (m_pItem)
	{
		D3DXMATRIX matItemScale, matItemTrans, matItemWorld;
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ITEM", dynamic_cast<CItem*>(m_pItem)->Get_StateKey(), dynamic_cast<CItem*>(m_pItem)->Get_ItemNum());

		assert(!(pTexInfo == nullptr));

		float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

		D3DXMatrixScaling(&matItemScale, 2.5f, 2.5f, 0.f);
		D3DXMatrixTranslation(&matItemTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 30.f, 0.f);

		matItemWorld = matItemScale * matItemTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matItemWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iItemAlpha, 255, 255, 255));

		// Input Price
		D3DXMATRIX	matPriceTrans;
		D3DXMatrixTranslation(&matPriceTrans, m_tInfo.vPos.x - 10.f, m_tInfo.vPos.y - 10.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matPriceTrans);

		swprintf_s(m_szPrice, L"%d", dynamic_cast<CItem*>(m_pItem)->Get_InputPrice());
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPrice, lstrlen(m_szPrice), nullptr, 0, D3DCOLOR_ARGB(m_iItemAlpha, 255, 255, 255));


	}


#ifdef _DEBUG
	// 충돌박스 출력
	D3DXVECTOR2 vLine[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};


	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// Original Price
	if (m_pItem)
	{
		D3DXMATRIX	matOriginTrans;
		D3DXMatrixTranslation(&matOriginTrans, m_tInfo.vPos.x - 10.f, m_tInfo.vPos.y, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matOriginTrans);

		swprintf_s(m_szOrigin, L"%d", dynamic_cast<CItem*>(m_pItem)->Get_Price());
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szOrigin, lstrlen(m_szOrigin), nullptr, 0, D3DCOLOR_ARGB(m_iItemAlpha, 255, 0, 0));
	}
#endif

}

void CShopTable::Release_Object()
{
}

int CShopTable::Get_OriginalPrice()
{
	int iOriginalPrice = dynamic_cast<CItem*>(m_pItem)->Get_Price();
	return iOriginalPrice;
}

int CShopTable::Get_InputPrice()
{
	int iInputPrice = dynamic_cast<CItem*>(m_pItem)->Get_InputPrice();
	return iInputPrice;
}
