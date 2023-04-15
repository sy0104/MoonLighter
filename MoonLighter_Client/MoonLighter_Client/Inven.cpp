#include "stdafx.h"
#include "Inven.h"
#include "Item.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "InvenUI.h"

#include "Showcase.h"
#include "Item.h"
#include "Sell.h"
#include "ShopTable.h"


CInven::CInven()
	: m_wstrStateKey(L""), m_iItemNum(0), m_bRender(false), m_pItem(nullptr), m_pInvenUI(nullptr), m_pShowcase(nullptr)
	, m_bShowcase(false), m_iShowcaseIdx(0), m_pSellButton(nullptr)
{
}

CInven::~CInven()
{
	Release_Object();
}

HRESULT CInven::Ready_Object()
{
	// 배경 삽입
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/Inventory.png", L"Inven")))
	{
		ERR_MSG(L"Inventory Insert Failed");
	}

	m_tInfo.vPos = { 100.f, 100.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iAlpha = 0;

	

	m_pInvenUI = CAbstractFactory<CInvenUI>::Create(m_tInfo.vPos.x + 123.f, m_tInfo.vPos.y + 200.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI2, m_pInvenUI);


	m_vecInven.reserve(15);

	//// Inven ////
	for (size_t i = 0; i < 15; ++i)
	{
		CObj* pItem = new CItem;
		m_vecInven.emplace_back(pItem);
	}

	// Golem Items
	for (size_t i = 0; i < 5; ++i)
	{
		dynamic_cast<CItem*>(m_vecInven[i])->Set_StateKey(L"Golem_Items");
		dynamic_cast<CItem*>(m_vecInven[i])->Set_ItemNum(i);
	}
	
	// Plant Items
	for (size_t i = 5; i < 10; ++i)
	{
		dynamic_cast<CItem*>(m_vecInven[i])->Set_StateKey(L"Plant_Items");
		dynamic_cast<CItem*>(m_vecInven[i])->Set_ItemNum(i - 5);
	}

	// Tech Itmes
	for (size_t i = 10; i < 15; ++i)
	{
		dynamic_cast<CItem*>(m_vecInven[i])->Set_StateKey(L"Tech_Items");
		dynamic_cast<CItem*>(m_vecInven[i])->Set_ItemNum(i - 10);
	}

	//// Showcase ////
	CObj* pObj = CAbstractFactory<CShowcase>::Create(730.f, 250.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI2, pObj);
	m_vecShowcase.emplace_back(pObj);

	pObj = CAbstractFactory<CShowcase>::Create(980.f, 250.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI2, pObj);
	m_vecShowcase.emplace_back(pObj);

	pObj = CAbstractFactory<CShowcase>::Create(730.f, 480.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI2, pObj);
	m_vecShowcase.emplace_back(pObj);

	pObj = CAbstractFactory<CShowcase>::Create(980.f, 480.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI2, pObj);
	m_vecShowcase.emplace_back(pObj);


	//// Sell Button ////
	m_pSellButton = CAbstractFactory<CSell>::Create(1060.f, 630.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, m_pSellButton);

	return S_OK;
}

int CInven::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	
	// Render //
	if (m_iAlpha == 255)
		dynamic_cast<CInvenUI*>(m_pInvenUI)->Set_Render();
	else if (m_iAlpha == 0)
		dynamic_cast<CInvenUI*>(m_pInvenUI)->Set_Hide();



	Key_Check();
	Sell_Item();

	return OBJ_NOEVENT;
}

void CInven::LateUpdate_Object()
{
}

void CInven::Render_Object()
{
	// Inven
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"Inven");
	assert(!(pTexInfo == nullptr));

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));


	// Coin
	D3DXMATRIX	matCoinTrans, matCoinWorld;
	D3DXMatrixTranslation(&matCoinTrans, m_tInfo.vPos.x + 240.f, m_tInfo.vPos.y + 490.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matCoinTrans);
	
	swprintf_s(m_szPlayerCoin, L"%d", m_iGold);

	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPlayerCoin, lstrlen(m_szPlayerCoin), nullptr, 0, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));


	// Item
	if (!m_vecInven.empty())
	{
		float i = 0.f, j = 0.f;

		for (size_t k = 0; k < m_vecInven.size(); ++k)
		{
			if (dynamic_cast<CItem*>(m_vecInven[k])->Get_ItemCount() > 0)
			{

				if (k == 5 || k == 10 || k == 15)
				{
					i = 0;
					j += 70.f;
				}

				D3DXMATRIX matItemScale, matItemTrans, matItemWorld, matFontTrans;

				const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ITEM", dynamic_cast<CItem*>(m_vecInven[k])->Get_StateKey(), dynamic_cast<CItem*>(m_vecInven[k])->Get_ItemNum());

				assert(!(pTexInfo == nullptr));

				float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
				float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

				D3DXMatrixScaling(&matItemScale, 2.5f, 2.5f, 0.f);
				D3DXMatrixTranslation(&matItemTrans, (float)(210 + i * 75), 300.f + j, 0.f);

				matItemWorld = matItemScale * matItemTrans;

				CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matItemWorld);
				CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

				// 아이템 개수
				D3DXMatrixTranslation(&matFontTrans, (float)(240 + i * 72), 310.f + j, 0.f);
				CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matFontTrans);

				swprintf_s(m_szItemCnt, L"%d", dynamic_cast<CItem*>(m_vecInven[k])->Get_ItemCount());
				CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szItemCnt, lstrlen(m_szItemCnt), nullptr, 0, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

				++i;
			}
		}
	}
}

void CInven::Release_Object()
{
	for (auto& pItem : m_vecInven)
		Safe_Delete(pItem);

	m_vecInven.clear();
}


void CInven::Insert_Item(CObj* _pItem)
{
	for (auto& iter = m_vecInven.begin(); iter != m_vecInven.end(); ++iter)
	{
		if (dynamic_cast<CItem*>(*iter)->Get_StateKey() == dynamic_cast<CItem*>(_pItem)->Get_StateKey() &&
			dynamic_cast<CItem*>(*iter)->Get_ItemNum() == dynamic_cast<CItem*>(_pItem)->Get_ItemNum())
		{
			dynamic_cast<CItem*>(*iter)->Set_ItemCount();
			return;
		}
	}


	CItem* pItem = new CItem;
	pItem->Set_StateKey(dynamic_cast<CItem*>(_pItem)->Get_StateKey());
	pItem->Set_ItemNum(dynamic_cast<CItem*>(_pItem)->Get_ItemNum());

	m_vecInven.emplace_back(pItem);
}

void CInven::Insert_Table(CObj* _pTable)
{
	m_vecTable.emplace_back(_pTable);
}

void CInven::Sell_Item()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		if (m_iShowcaseIdx > 3)
			return;

		// Golem Items
		if (m_pInvenUI->Get_Info().vPos.y == 300.f)
		{
			if (m_pInvenUI->Get_Info().vPos.x == 223.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[0]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(500);
				dynamic_cast<CItem*>(m_vecInven[0])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 295.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[1]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1000);
				dynamic_cast<CItem*>(m_vecInven[1])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 367.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[2]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1500);
				dynamic_cast<CItem*>(m_vecInven[2])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 439.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[3]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2000);
				dynamic_cast<CItem*>(m_vecInven[3])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 511.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[4]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2500);
				dynamic_cast<CItem*>(m_vecInven[4])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
		}

		// Plant Items
		else if (m_pInvenUI->Get_Info().vPos.y == 370.f)
		{
			if (m_pInvenUI->Get_Info().vPos.x == 223.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[5]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(500);
				dynamic_cast<CItem*>(m_vecInven[5])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 295.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[6]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1000);
				dynamic_cast<CItem*>(m_vecInven[6])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 367.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[7]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1500);
				dynamic_cast<CItem*>(m_vecInven[7])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 439.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[8]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2000);
				dynamic_cast<CItem*>(m_vecInven[8])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 511.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[9]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2500);
				dynamic_cast<CItem*>(m_vecInven[9])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}

		}

		// Tech Items
		else if (m_pInvenUI->Get_Info().vPos.y == 440.f)
		{
			if (m_pInvenUI->Get_Info().vPos.x == 223.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[10]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(500);
				dynamic_cast<CItem*>(m_vecInven[10])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 295.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[11]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1000);
				dynamic_cast<CItem*>(m_vecInven[11])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 367.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[12]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(1500);
				dynamic_cast<CItem*>(m_vecInven[12])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 439.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[13]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2000);
				dynamic_cast<CItem*>(m_vecInven[13])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
			else if (m_pInvenUI->Get_Info().vPos.x == 511.f)
			{
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Item(m_vecInven[14]);
				dynamic_cast<CShowcase*>(m_vecShowcase[m_iShowcaseIdx])->Set_Price(2500);
				dynamic_cast<CItem*>(m_vecInven[14])->Set_ItemCountMinus();
				++m_iShowcaseIdx;
			}
		}
	}
}

void CInven::Key_Check()
{
	//// Alpha값 조절 ////
	if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		if (!m_bShowcase)
		{
			for (auto& iter = m_vecShowcase.begin(); iter != m_vecShowcase.end(); ++iter)
				dynamic_cast<CShowcase*>(*iter)->Set_Render();

			dynamic_cast<CSell*>(m_pSellButton)->Set_Render();

			m_bShowcase = true;
		}

		else if (m_bShowcase)
		{
			for (auto& iter = m_vecShowcase.begin(); iter != m_vecShowcase.end(); ++iter)
				dynamic_cast<CShowcase*>(*iter)->Set_Hide();

			dynamic_cast<CSell*>(m_pSellButton)->Set_Hide();

			m_bShowcase = false;
		}

	}

	// -------------------------------------------------------------------------------------------

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	//// Sell Button ////
	if (pt.x >= m_pSellButton->Get_Rect().left && pt.x <= m_pSellButton->Get_Rect().right
		&& pt.y >= m_pSellButton->Get_Rect().top && pt.y <= m_pSellButton->Get_Rect().bottom)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON) ||
			CKeyMgr::Get_Instance()->Key_Down(3))
		{
			// Table에 Item 넣어줌
			//for (size_t i = 0; i < 4; ++i)
			//	dynamic_cast<CShopTable*>(m_vecTable[i])->Set_Item(dynamic_cast<CShowcase*>(m_vecShowcase[i])->Get_Item());

			for (size_t i = 0; i < 4; ++i)
			{
				CObj* pItem = new CItem;
				dynamic_cast<CItem*>(pItem)->Set_StateKey(dynamic_cast<CShowcase*>(m_vecShowcase[i])->Get_StateKey());
				dynamic_cast<CItem*>(pItem)->Set_ItemNum(dynamic_cast<CShowcase*>(m_vecShowcase[i])->Get_ItemNum());
				dynamic_cast<CItem*>(pItem)->Set_Price(dynamic_cast<CShowcase*>(m_vecShowcase[i])->Get_OriginalPrice());
				dynamic_cast<CItem*>(pItem)->Set_InputPrice(dynamic_cast<CShowcase*>(m_vecShowcase[i])->Get_InputPrice());

				dynamic_cast<CShopTable*>(m_vecTable[i])->Set_Item(pItem);
			}	
		}

		else if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		{
			// Showcase에 있는 아이템 삭제
			for (auto& iter = m_vecShowcase.begin(); iter != m_vecShowcase.end(); ++iter)
				dynamic_cast<CShowcase*>(*iter)->Set_Hide();
				//dynamic_cast<CShowcase*>(*iter)->Delete_Item();
			dynamic_cast<CSell*>(m_pSellButton)->Set_Hide();
			m_iAlpha = 0;
		}

	}
}
