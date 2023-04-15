#include "stdafx.h"
#include "Showcase.h"
#include "Graphic_Device.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "Item.h"

#include "Inven.h"
#include "ObjMgr.h"
#include "PriceDown.h"
#include "PriceUp.h"
#include "Sell.h"

CShowcase::CShowcase()
	: m_pItem(nullptr), m_pPriceUp(nullptr), m_pPriceDown(nullptr), m_bCreateButton(false)
{
}


CShowcase::~CShowcase()
{
	Release_Object();
}

HRESULT CShowcase::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Showcase.png", L"Showcase")))
	{
		ERR_MSG(L"Table Insert Failed");
	}

	m_iAlpha = 0;


	return S_OK;
}

int CShowcase::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCreateButton)
	{
		m_pPriceUp = CAbstractFactory<CPriceUp>::Create(m_tInfo.vPos.x - 25.f, m_tInfo.vPos.y + 10.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, m_pPriceUp);
		
		m_pPriceDown = CAbstractFactory<CPriceDown>::Create(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y + 10.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, m_pPriceDown);


		m_bCreateButton = true;
	}


	if (m_iAlpha == 0)
	{
		dynamic_cast<CPriceUp*>(m_pPriceUp)->Set_Hide();
		dynamic_cast<CPriceDown*>(m_pPriceDown)->Set_Hide();
	}

	else if (m_iAlpha == 255)
	{
		dynamic_cast<CPriceUp*>(m_pPriceUp)->Set_Render();
		dynamic_cast<CPriceDown*>(m_pPriceDown)->Set_Render();
	}

	Key_Check();

	return OBJ_NOEVENT;
}

void CShowcase::LateUpdate_Object()
{
}

void CShowcase::Render_Object()
{
	// Showcase
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Showcase");

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Item
	if (m_pItem)
	{
		D3DXMATRIX matItemScale, matItemTrans, matItemWorld;
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ITEM", dynamic_cast<CItem*>(m_pItem)->Get_StateKey(), dynamic_cast<CItem*>(m_pItem)->Get_ItemNum());

		assert(!(pTexInfo == nullptr));

		float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

		D3DXMatrixScaling(&matItemScale, 2.5f, 2.5f, 0.f); 
		D3DXMatrixTranslation(&matItemTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 65.f, 0.f);

		matItemWorld = matItemScale * matItemTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matItemWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}

	// Item Price
	if (m_pItem)
	{
		D3DXMATRIX	matPriceTrans;
		D3DXMatrixTranslation(&matPriceTrans, m_tInfo.vPos.x + 15.f, m_tInfo.vPos.y + 50.f, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matPriceTrans);

		swprintf_s(m_szPrice, L"%d", dynamic_cast<CItem*>(m_pItem)->Get_InputPrice());
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPrice, lstrlen(m_szPrice), nullptr, 0, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));
	}
}

void CShowcase::Release_Object()
{
}

wstring CShowcase::Get_StateKey()
{
	wstring wstrStateKey = dynamic_cast<CItem*>(m_pItem)->Get_StateKey();
	return wstrStateKey;
}

int CShowcase::Get_ItemNum()
{
	int iItemNum = dynamic_cast<CItem*>(m_pItem)->Get_ItemNum();
	return iItemNum;
}

int CShowcase::Get_InputPrice()
{
	int iInputPrice = dynamic_cast<CItem*>(m_pItem)->Get_InputPrice();
	return iInputPrice;
}

int CShowcase::Get_OriginalPrice()
{
	int iPrice = dynamic_cast<CItem*>(m_pItem)->Get_Price();
	return iPrice;
}

void CShowcase::Set_Price(int iPrice)
{
	dynamic_cast<CItem*>(m_pItem)->Set_Price(iPrice);
}

void CShowcase::Key_Check()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	//// Price Button ////
	if (pt.x >= m_pPriceUp->Get_Rect().left && pt.x <= m_pPriceUp->Get_Rect().right
		&& pt.y >= m_pPriceUp->Get_Rect().top && pt.y <= m_pPriceUp->Get_Rect().bottom)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) ||
			CKeyMgr::Get_Instance()->Key_Down(1))
			dynamic_cast<CItem*>(m_pItem)->Set_InputPrice(100);
	}

	else if (pt.x >= m_pPriceDown->Get_Rect().left && pt.x <= m_pPriceDown->Get_Rect().right
		&& pt.y >= m_pPriceDown->Get_Rect().top && pt.y <= m_pPriceDown->Get_Rect().bottom)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) ||
			CKeyMgr::Get_Instance()->Key_Down(2))
			dynamic_cast<CItem*>(m_pItem)->Set_InputPrice(-100);
	}

}
