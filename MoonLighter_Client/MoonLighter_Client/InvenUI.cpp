#include "stdafx.h"
#include "InvenUI.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "KeyMgr.h"



CInvenUI::CInvenUI()
{
}


CInvenUI::~CInvenUI()
{
	Release_Object();
}

HRESULT CInvenUI::Ready_Object()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(CTextureMgr::TEX_SINGLE, L"../Texture/SingleTex/InvenUI.png", L"InvenUI")))
	{
		ERR_MSG(L"InventoryUI Insert Failed");
	}

	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iAlpha = 255;


	return S_OK;
}

int CInvenUI::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Check();

	return OBJ_NOEVENT;
}

void CInvenUI::LateUpdate_Object()
{
}

void CInvenUI::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"InvenUI");
	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

//#ifdef _DEBUG
//	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
//	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
//
//	swprintf_s(m_szPos, L"(%f, %f)", m_tInfo.vPos.x, m_tInfo.vPos.y);
//	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPos, lstrlen(m_szPos), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
//
//#endif
}


void CInvenUI::Release_Object()
{
}

void CInvenUI::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Down('Y'))
		m_tInfo.vPos.y -= 70.f;
	else if (CKeyMgr::Get_Instance()->Key_Down('G'))
		m_tInfo.vPos.x -= 72.f;
	else if (CKeyMgr::Get_Instance()->Key_Down('H'))
		m_tInfo.vPos.y += 70.f;
	else if (CKeyMgr::Get_Instance()->Key_Down('J'))
		m_tInfo.vPos.x += 72.f;

}
