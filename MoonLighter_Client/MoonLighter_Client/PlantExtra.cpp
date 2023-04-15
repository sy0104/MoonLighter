#include "stdafx.h"
#include "PlantExtra.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "SoundMgr.h"

CPlantExtra::CPlantExtra()
	: m_wstrStateKey(L"")
{
}


CPlantExtra::~CPlantExtra()
{
	Release_Object();
}

HRESULT CPlantExtra::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	CSoundMgr::Get_Instance()->PlaySound(L"Plant_WakeUp.wav", CSoundMgr::EFFECT);


	return S_OK;
}

int CPlantExtra::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	Frame_Move(0.1);

	return OBJ_NOEVENT;
}

void CPlantExtra::LateUpdate_Object()
{
}

void CPlantExtra::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_B", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlantExtra::Release_Object()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
}

void CPlantExtra::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
	{
		m_bDead = true;
	}
}
