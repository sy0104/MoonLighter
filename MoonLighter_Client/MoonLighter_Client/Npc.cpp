#include "stdafx.h"
#include "Npc.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "Reaction.h"
#include "SoundMgr.h"

#include "ShopTable.h"
#include "Inven.h"
#include "Item.h"


CNpc::CNpc()
	: m_ePreScene(END), m_eCurScene(END), m_wstrObjectKey(L""), m_wstrStateKey(L""), m_bWalk(false), m_bEnter(false), m_bBuy(false)
	, m_dNathanTime(0.0), m_bNathanBreak(false), m_bNathanUp(false), m_bNathanReaction(false), m_bNathanBuy(false), m_bNathanGold(false)
	, m_dSakuraTime(0.0), m_bSakuraBreak(false), m_bSakuraLeft(false), m_bSakuraReaction(false), m_bSakuraBuy(false), m_bSakuraGold(false)
	, m_dWorkerTime(0.0), m_bWorkerBreak(false), m_bWorkerDown(false), m_bWorkerReaction(false), m_bWorkerBuy(false), m_bWorkerGold(false)
	, m_dRiseTime(0.0), m_bRiseReaction(false), m_bRiseBuy(false), m_bRiseGold(false)
{
}


CNpc::~CNpc()
{
	Release_Object();
}

HRESULT CNpc::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 2.f;


	return S_OK;
}

int CNpc::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	// Town
	if (m_bWalk)
	{
		if (m_wstrObjectKey == L"Nathan")
			Nathan_Walk();
		else if (m_wstrObjectKey == L"Sakura")
			Sakura_Walk();
		else if (m_wstrObjectKey == L"Worker")
			Worker_Walk();
	}

	// Shop
	else
	{
		if (m_wstrObjectKey == L"Nathan")
			Nathan_Shop();
		else if (m_wstrObjectKey == L"Sakura")
			Sakura_Shop();
		else if (m_wstrObjectKey == L"Worker")
			Worker_Shop();
		else if (m_wstrObjectKey == L"Rise")
			Rise_Shop();
	}

	// Inven
	m_pInven = CObjMgr::Get_Instance()->Get_List(OBJ::INVEN).front();

	// Table
	m_vecTable = dynamic_cast<CInven*>(m_pInven)->Get_Table();

	Frame_Move();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CNpc::LateUpdate_Object()
{
}

void CNpc::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CNpc::Release_Object()
{
}


void CNpc::Insert_Table(CObj* _pTable)
{
	m_vecTable.emplace_back(_pTable);
}

void CNpc::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		//// IDLE ////
		case IDLE_LEFT:
			m_wstrStateKey = L"IDLE_LEFT";
			m_tFrame = { 0.0, 0.0 };
			break;
		case IDLE_RIGHT:
			m_wstrStateKey = L"IDLE_RIGHT";
			m_tFrame = { 0.0, 0.0 };
			break;
		case IDLE_DOWN:
			m_wstrStateKey = L"IDLE_DOWN";
			m_tFrame = { 0.0, 0.0 };
			break;
		case IDLE_UP:
			m_wstrStateKey = L"IDLE_UP";
			m_tFrame = { 0.0, 0.0 };
			break;

		//// WALK ////
		case WALK_LEFT:
			m_wstrStateKey = L"WALK_LEFT";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_RIGHT:
			m_wstrStateKey = L"WALK_RIGHT";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_UP:
			m_wstrStateKey = L"WALK_UP";
			m_tFrame = { 0.0, 8.0 };
			break;
		case WALK_DOWN:
			m_wstrStateKey = L"WALK_DOWN";
			m_tFrame = { 0.0, 8.0 };
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}

bool CNpc::Reaction(int iIdx)
{
	int iOriginalPrice = dynamic_cast<CShopTable*>(m_vecTable[iIdx])->Get_OriginalPrice();
	int iInputPrice = dynamic_cast<CShopTable*>(m_vecTable[iIdx])->Get_InputPrice();

	// ¿ø·¡ °¡°Ýº¸´Ù ºñ½Ò¶§: Doubt -> Too Expensive
	if (iOriginalPrice < iInputPrice)
	{
		CObj* pObj = CAbstractFactory<CReaction>::Create(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y - 40.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, pObj);
		dynamic_cast<CReaction*>(pObj)->Set_State(L"Doubt");
		dynamic_cast<CReaction*>(pObj)->Set_SecondState(L"TooExpensive");
		dynamic_cast<CReaction*>(pObj)->Set_Frame(18.0);
		dynamic_cast<CReaction*>(pObj)->Set_SecondFrame(6.0);

		return false;
	}

	// ¿ø·¡ °¡°Ýº¸´Ù ½Ò¶§: Alert -> Too Cheap
	else if (iOriginalPrice > iInputPrice)
	{
		CObj* pObj = CAbstractFactory<CReaction>::Create(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y - 40.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, pObj);
		dynamic_cast<CReaction*>(pObj)->Set_State(L"Alert");
		dynamic_cast<CReaction*>(pObj)->Set_SecondState(L"TooCheap");
		dynamic_cast<CReaction*>(pObj)->Set_Frame(6.0);
		dynamic_cast<CReaction*>(pObj)->Set_SecondFrame(10.0);

		return true;
	}

	// ¿ø·¡ °¡°ÝÀÌ¶û ºñ½ÁÇÒ¶§: Think -> Transition
	else if (iOriginalPrice == iInputPrice)
	{
		CObj* pObj = CAbstractFactory<CReaction>::Create(m_tInfo.vPos.x + 25.f, m_tInfo.vPos.y - 40.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, pObj);
		dynamic_cast<CReaction*>(pObj)->Set_State(L"Think");
		dynamic_cast<CReaction*>(pObj)->Set_SecondState(L"Transition");
		dynamic_cast<CReaction*>(pObj)->Set_Frame(2.0);
		dynamic_cast<CReaction*>(pObj)->Set_SecondFrame(2.0);

		return true;
	}
}

void CNpc::Nathan_Walk()
{
	// (950.f, 350.f), WALK_DOWN

	if (!m_bNathanBreak)
	{
		// À§·Î
		if (!m_bNathanUp)
		{
			if (m_tInfo.vPos.y <= 600.f)
			{
				m_eCurScene = WALK_DOWN;
				m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
			}

			else if (m_tInfo.vPos.y > 600.f)
			{
				m_bNathanUp = true;
				m_bNathanBreak = true;
			}
		}

		// À§·Î
		else if (m_bNathanUp)
		{
			if (m_tInfo.vPos.y > 350.f)
			{
				m_eCurScene = WALK_UP;

				m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
			}
			else if (m_tInfo.vPos.y <= 350.f)
			{
				m_bNathanUp = false;
				m_bNathanBreak = true;
			}
		}
	}


	// ¸ØÃã
	else if (m_bNathanBreak)
	{
		m_dNathanTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dNathanTime >= 1.0)
			m_eCurScene = IDLE_LEFT;
		else if (m_dNathanTime >= 2.0)
			m_eCurScene = IDLE_RIGHT;
		else
			m_eCurScene = IDLE_DOWN;

		if (m_dNathanTime >= 3.0)
		{
			m_bNathanBreak = false;
			m_dNathanTime = 0.0;
		}
	}
}

void CNpc::Sakura_Walk()
{
	// (400.f, 245.f), WALK_LEFT

	if (!m_bSakuraBreak)
	{
		// ¿ÞÂÊ
		if (!m_bSakuraLeft)
		{
			if (m_tInfo.vPos.x >= 300.f)
			{
				m_eCurScene = WALK_LEFT;
				m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
			}
			else if (m_tInfo.vPos.x < 300.f)
			{
				m_bSakuraLeft = true;
				m_bSakuraBreak = true;
			}
		}

		// ¿À¸¥ÂÊ
		else if (m_bSakuraLeft)
		{
			if (m_tInfo.vPos.x <= 700.f)
			{
				m_eCurScene = WALK_RIGHT;
				m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
			}
			else if (m_tInfo.vPos.x > 700.f)
			{
				m_bSakuraLeft = false;
				m_bSakuraBreak = true;
			}
		}
	}

	// ¸ØÃã
	else if (m_bSakuraBreak)
	{
		m_dSakuraTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dSakuraTime >= 4.0)
		{
			m_bSakuraBreak = false;
			m_dSakuraTime = 0.0;
		}
		else
			m_eCurScene = IDLE_DOWN;
	}
}

void CNpc::Worker_Walk()
{
	// (145.f, 600.f), WALK_UP

	m_fSpeed = 1.5f;


	if (!m_bWorkerBreak)
	{
		// À§
		if (!m_bWorkerDown)
		{
			if (m_tInfo.vPos.y >= 300.f)
			{
				m_eCurScene = WALK_UP;
				m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
			}
			else if (m_tInfo.vPos.y < 300.f)
			{
				m_bWorkerDown = true;
				m_bWorkerBreak = true;
			}
		}

		// ¾Æ·¡
		else if (m_bWorkerDown)
		{
			if (m_tInfo.vPos.y <= 700.f)
			{
				m_eCurScene = WALK_DOWN;
				m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
			}
			else if (m_tInfo.vPos.y > 700.f)
			{
				m_bWorkerDown = false;
				m_bWorkerBreak = true;
			}
		}
	}

	// ¸ØÃã
	else if (m_bWorkerBreak)
	{
		m_dWorkerTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dWorkerTime >= 3.0)
		{
			m_bWorkerBreak = false;
			m_dWorkerTime = 0.0;
		}
		else
			m_eCurScene = IDLE_RIGHT;
	}
}

void CNpc::Nathan_Shop()
{
	if (!m_bEnter)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Shop_NpcEnter.wav", CSoundMgr::EFFECT);
		m_dNathanTime = 0.0;
		m_bEnter = true;
	}

	if (!m_bNathanReaction)
	{
		// (670, 510) - UP
		if (m_tInfo.vPos.y >= 340.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		// (560, 340) - LEFT
		else if (m_tInfo.vPos.x >= 560.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		// (560, 170) - UP
		else if (m_tInfo.vPos.y >= 170.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		// (400, 170) - LEFT
		else if (m_tInfo.vPos.x >= 400.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		// (400, 170) - IDLE_DOWN
		else
		{
			m_eCurScene = IDLE_DOWN;

			m_bNathanBuy = Reaction(0);
			m_bNathanReaction = true;
			
		}
	}

	m_dNathanTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	
	if (m_dNathanTime >= 8.0 && m_bNathanReaction)
	{
		if (m_bNathanBuy)
		{
			if (!m_bNathanGold)
			{
				m_pInven->Set_Gold(dynamic_cast<CShopTable*>(m_vecTable[0])->Get_InputPrice());
				CSoundMgr::Get_Instance()->PlaySound(L"Shop_ItemSold.wav", CSoundMgr::EFFECT);
				m_bNathanGold = true;
			}
			dynamic_cast<CShopTable*>(m_vecTable[0])->Hide_Item();
		}
		

		if (m_tInfo.vPos.x <= 570.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 400.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
		}

		else if (m_tInfo.vPos.x <= 680.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 550.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;

			if (m_tInfo.vPos.y == 550.f)
				m_bDead = true;
		}
	}
}

void CNpc::Sakura_Shop()
{
	if (!m_bEnter)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Shop_NpcEnter.wav", CSoundMgr::EFFECT);
		m_bEnter = true;
	}

	if (!m_bSakuraReaction)
	{
		// (670, 510) - UP
		if (m_tInfo.vPos.y >= 340.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		// (560, 340) - LEFT
		else if (m_tInfo.vPos.x >= 560.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		// (560, 170) - UP
		else if (m_tInfo.vPos.y >= 170.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		// (400, 170) - LEFT
		else if (m_tInfo.vPos.x >= 480.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		// (400, 170) - IDLE_DOWN
		else
		{
			m_eCurScene = IDLE_DOWN;

			m_bSakuraBuy = Reaction(1);
			m_bSakuraReaction = true;
		}
	}

	m_dSakuraTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dSakuraTime >= 7.0 && m_bSakuraReaction)
	{
		if (m_bSakuraBuy)
		{ 
			if (!m_bSakuraGold)
			{
				m_pInven->Set_Gold(dynamic_cast<CShopTable*>(m_vecTable[1])->Get_InputPrice());
				CSoundMgr::Get_Instance()->PlaySound(L"Shop_ItemSold.wav", CSoundMgr::EFFECT);
				m_bSakuraGold = true;
			}

			dynamic_cast<CShopTable*>(m_vecTable[1])->Hide_Item();
		}

		if (m_tInfo.vPos.x <= 570.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 400.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;
		}

		else if (m_tInfo.vPos.x <= 680.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 550.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;

			if (m_tInfo.vPos.y == 550.f)
				m_bDead = true;
		}
	}
}

void CNpc::Worker_Shop()
{
	if (!m_bEnter)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Shop_NpcEnter.wav", CSoundMgr::EFFECT);
		m_bEnter = true;
	}

	if (!m_bWorkerReaction)
	{
		if (m_tInfo.vPos.y >= 380.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		else if (m_tInfo.vPos.x >= 390.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		else
		{
			m_eCurScene = IDLE_UP;
			m_bWorkerBuy = Reaction(2);

			m_bWorkerReaction = true;		
		}
	}

	m_dWorkerTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dWorkerTime >= 5.0 && m_bWorkerReaction)
	{
		if (m_bWorkerBuy)
		{
			if (!m_bWorkerGold)
			{
				m_pInven->Set_Gold(dynamic_cast<CShopTable*>(m_vecTable[2])->Get_InputPrice());
				CSoundMgr::Get_Instance()->PlaySound(L"Shop_ItemSold.wav", CSoundMgr::EFFECT);
				m_bWorkerGold = true;
			}
			dynamic_cast<CShopTable*>(m_vecTable[2])->Hide_Item();
		}

		if (m_tInfo.vPos.x <= 680.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 550.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;

			if (m_tInfo.vPos.y == 550.f)
				m_bDead = true;
		}
	}
}

void CNpc::Rise_Shop()
{
	if (!m_bEnter)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Shop_NpcEnter.wav", CSoundMgr::EFFECT);
		m_bEnter = true;
	}

	if (!m_bRiseReaction)
	{
		if (m_tInfo.vPos.y >= 380.f)
		{
			m_eCurScene = WALK_UP;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;
		}

		else if (m_tInfo.vPos.x >= 470.f)
		{
			m_eCurScene = WALK_LEFT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;
		}

		else
		{
			m_eCurScene = IDLE_UP;

			m_bRiseBuy = Reaction(3);
			m_bRiseReaction = true;
		}
	}

	m_dRiseTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dRiseTime >= 5.0 && m_bRiseReaction)
	{
		if (m_bRiseBuy)
		{
			if (!m_bRiseGold)
			{
				m_pInven->Set_Gold(dynamic_cast<CShopTable*>(m_vecTable[3])->Get_InputPrice());
				CSoundMgr::Get_Instance()->PlaySound(L"Shop_ItemSold.wav", CSoundMgr::EFFECT);
				m_bRiseGold = true;
			}
			dynamic_cast<CShopTable*>(m_vecTable[3])->Hide_Item();
		}


		if (m_tInfo.vPos.x <= 680.f)
		{
			m_eCurScene = WALK_RIGHT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;
		}

		else if (m_tInfo.vPos.y <= 550.f)
		{
			m_eCurScene = WALK_DOWN;
			m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;

			if (m_tInfo.vPos.y == 550.f)
				m_bDead = true;
		}
	}
}