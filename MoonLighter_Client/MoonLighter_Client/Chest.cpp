#include "stdafx.h"
#include "Chest.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Item.h"

CChest::CChest()
	: m_ePreScene(END), m_eCurScene(END), m_wstrStateKey(L""), m_bOpen(false), m_bDrop(false), m_bStart(false)
{
}


CChest::~CChest()
{
	Release_Object();
}

HRESULT CChest::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;
	m_wstrStateKey = L"IDLE";
	m_tFrame = { 0.0, 6.0 };

	return S_OK;
}

int CChest::Update_Object()
{
	if (m_bDead)
	{
		// Item
		for (int i = 0; i < 5; ++i)
		{
			CObj* pItem = CAbstractFactory<CItem>::Create((float)(rand() % (-200) + (m_tInfo.vPos.x - 100)), (float)(rand() % (-100) + (m_tInfo.vPos.y - 50)));
			pItem->Set_AlphaValue(255);
			CObjMgr::Get_Instance()->Add_Object(OBJ::ITEM, pItem);
			dynamic_cast<CItem*>(pItem)->Set_StateKey(L"Golem_Items");
		}

		return OBJ_DEAD;
	}

	//if (m_bOpen && !m_bStart)
	//{
	//	//m_tFrame.dFrameStart = 0.0;
	//	m_eCurScene = OPEN;
	//	m_wstrStateKey = L"OPEN";
	//	m_tFrame = { 0.0, 5.0 };

	//	m_bStart = true;
	//}

	if (m_bDrop)
	{
		m_iAlpha -= 5;
		if (m_iAlpha <= 0)
			m_bDead = true;
	}

	//Scene_Change();

	if (m_bOpen)
		Frame_Move();

	return OBJ_NOEVENT;
}

void CChest::LateUpdate_Object()
{
}

void CChest::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"CHEST", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));

#ifdef _DEBUG
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

#endif

}

void CChest::Release_Object()
{
}

void CChest::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;


	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
	{
		//if (m_eCurScene == IDLE)
		//	m_tFrame.dFrameStart = 0.0;

		//else if (m_eCurScene == OPEN)
		//{
		//	//m_tFrame.dFrameStart = 0.0;

		//	m_bDrop = true;
		//}
		m_tFrame.dFrameStart = 6.0;
		m_bOpen = false;
		m_bDrop = true;
	}
}

void CChest::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 0.0 };
			break;
		case OPEN:
			m_wstrStateKey = L"OPEN";
			m_tFrame = { 0.0, 5.0 };
		}
	}
}
