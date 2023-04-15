#include "stdafx.h"
#include "BossHpBar.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CBossHpBar::CBossHpBar()
{
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
}


CBossHpBar::~CBossHpBar()
{
	Release_Object();
}

HRESULT CBossHpBar::Ready_Object()
{


	return S_OK;
}

int CBossHpBar::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;



	return OBJ_NOEVENT;
}

void CBossHpBar::LateUpdate_Object()
{
}

void CBossHpBar::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"HEAL_BAR", L"Boss_Hp", 0);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBossHpBar::Release_Object()
{
}
