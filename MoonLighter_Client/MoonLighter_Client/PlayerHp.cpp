#include "stdafx.h"
#include "PlayerHp.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

CPlayerHp::CPlayerHp()
{
}


CPlayerHp::~CPlayerHp()
{
	Release_Object();
}

HRESULT CPlayerHp::Ready_Object()
{
	m_tInfo.vPos = { 120.f, 50.f, 0.f };
	m_tInfo.vSize = { 1.8f, 2.f, 0.f };

	m_tHpRect = { (LONG)0, (LONG)0, (LONG)10, (LONG)12 };

	return S_OK;
}

int CPlayerHp::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CPlayerHp::LateUpdate_Object()
{
}

void CPlayerHp::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"HEAL_BAR", L"Player_Hp", 1);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &m_tHpRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerHp::Release_Object()
{
}
