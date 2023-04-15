#include "stdafx.h"
#include "PlayerHpBar.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CPlayerHpBar::CPlayerHpBar()
{
	m_tInfo.vSize = { 3.f, 2.f, 0.f };
}


CPlayerHpBar::~CPlayerHpBar()
{
	Release_Object();
}

HRESULT CPlayerHpBar::Ready_Object()
{
	return S_OK;
}

int CPlayerHpBar::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;




	return OBJ_NOEVENT;
}

void CPlayerHpBar::LateUpdate_Object()
{
}

void CPlayerHpBar::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"HEAL_BAR", L"Player_Hp", 0);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayerHpBar::Release_Object()
{
}
