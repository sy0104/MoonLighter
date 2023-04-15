#include "stdafx.h"
#include "Item.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "SceneMgr.h"


CItem::CItem()
	: m_wstrStateKey(L""), m_iItemNum(0), m_bCreated(false), m_iCount(0), m_iPrice(0), m_iInputPrice(0)
{
}


CItem::~CItem()
{
	Release_Object();
}

HRESULT CItem::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.5f, 2.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iAlpha = 0;

	m_iItemNum = rand() % 5;
	m_iPrice = ((m_iItemNum + 1) * 500);

	return S_OK;
}

int CItem::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCreated && m_iAlpha < 255)
	{
		m_iAlpha += 5;
	}
	else if (m_iAlpha >= 255)
		m_bCreated = true;

	return OBJ_NOEVENT;
}

void CItem::LateUpdate_Object()
{
}

void CItem::Render_Object()
{
	if (CSceneMgr::Get_Instance()->Get_Scroll())
	{
		D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

		D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ITEM", m_wstrStateKey, m_iItemNum);

		assert(!(pTexInfo == nullptr));

		float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

		matWorld = matScale * matRotZ * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		// Rect
		m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 2.f));
		m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 2.f));
		m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 2.f));
		m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 2.f));


#ifdef _DEBUG
		// 충돌박스 출력
		D3DXVECTOR2 vLine[5] =
		{
			{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y },
			{ (float)m_tRect.right + vScroll.x, (float)m_tRect.top + vScroll.y },
			{ (float)m_tRect.right + vScroll.x, (float)m_tRect.bottom + vScroll.y },
			{ (float)m_tRect.left + vScroll.x, (float)m_tRect.bottom + vScroll.y },
			{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y }
		};

		CGraphic_Device::Get_Instance()->Get_Sprite()->End();
		CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif
	}

	else
	{
		D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"ITEM", m_wstrStateKey, m_iItemNum);

		assert(!(pTexInfo == nullptr));

		float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		matWorld = matScale * matRotZ * matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		// Rect
		m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 2.f));
		m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 2.f));
		m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 2.f));
		m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 2.f));

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
#endif
	}
}

void CItem::Release_Object()
{
}

void CItem::Set_InputPrice(int iInput)
{
	/*if (m_iInputPrice <= 0)
		return;*/

	m_iInputPrice += iInput;
}
