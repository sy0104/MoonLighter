#include "stdafx.h"
#include "Ui.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"


CUi::CUi()
	: m_ePreID(UI::END), m_eCurID(UI::END), m_wstrObjectKey(L""), m_wstrStateKey(L""), m_iCount(0)
{
}


CUi::~CUi()
{
	Release_Object();
}

HRESULT CUi::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };


	return S_OK;
}

int CUi::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	Scene_Change();

	return OBJ_NOEVENT;
}

void CUi::LateUpdate_Object()
{
}

void CUi::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_iCount);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CUi::Release_Object()
{
}

void CUi::Scene_Change()
{
	if (m_ePreID != m_eCurID)
	{
		switch (m_eCurID)
		{
		case UI::PLAYER_HPBAR:
			m_wstrObjectKey = L"HEAL_BAR";
			m_wstrStateKey = L"Player_Hp";
			m_iCount = 0;
			break;
		case UI::PLAYER_HP:
			m_wstrObjectKey = L"HEAL_BAR";
			m_wstrStateKey = L"Player_Hp";
			m_iCount = 1;
			break;
		case UI::BOSS_HPBAR:
			m_wstrObjectKey = L"HEAL_BAR";
			m_wstrStateKey = L"Boss_Hp";
			m_iCount = 0;
			break;
		case UI::BOSS_HP:
			m_wstrObjectKey = L"HEAL_BAR";
			m_wstrStateKey = L"Boss_Hp";
			m_iCount = 1;
			break;
		}

		m_ePreID = m_eCurID;
	}
}
