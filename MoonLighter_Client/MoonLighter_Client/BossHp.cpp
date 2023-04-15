#include "stdafx.h"
#include "BossHp.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CBossHp::CBossHp()
{
	ZeroMemory(&m_tHpRect, sizeof(m_tHpRect));
}


CBossHp::~CBossHp()
{
	Release_Object();
}

HRESULT CBossHp::Ready_Object()
{
	m_tInfo.vPos = { 1400.f, 750.f, 0.f };
	m_tInfo.vSize = { 5.7f, 2.f, 0.f };

	m_tHpRect = { (LONG)0, (LONG)0, (LONG)432, (LONG)6 }; 

	return S_OK;
}

int CBossHp::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CBossHp::LateUpdate_Object()
{
}

void CBossHp::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"HEAL_BAR", L"Boss_Hp", 1);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &m_tHpRect, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBossHp::Release_Object()
{
}
