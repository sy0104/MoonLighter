#include "stdafx.h"
#include "Effect.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"

#include "SoundMgr.h"


CEffect::CEffect()
	: m_ePreID(EFFECT::END), m_eCurID(EFFECT::END), m_wstrObjectKey(L""), m_wstrStateKey(L""), m_dFrameSpeed(0.0)
	, m_bCheck(false), m_bSound(false), m_dSoundTime(0.0)
{
}


CEffect::~CEffect()
{
	Release_Object();
}

HRESULT CEffect::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	return S_OK;
}

int CEffect::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	if (m_eCurID == EFFECT::ROCK_EFFECT)
		CSoundMgr::Get_Instance()->PlaySound(L"Golem_RockDeath.wav", CSoundMgr::EFFECT);



	Frame_Move(m_dFrameSpeed);
	Scene_Change();

	return OBJ_NOEVENT;
}

void CEffect::LateUpdate_Object()
{
	if (m_eCurID == EFFECT::DASH && !m_bCheck)
	{
		float fX = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.x - m_tInfo.vPos.x;
		float fY = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y - m_tInfo.vPos.y;

		float fDia = sqrtf(fX * fX + fY * fY);

		float fRad = acosf(fX / fDia);
		m_fAngle = D3DXToDegree(fRad);

		if (m_tInfo.vPos.y > CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos.y)
			m_fAngle *= -1;

		m_bCheck = true;
	}
}

void CEffect::Render_Object()
{
	D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	if (m_eCurID == EFFECT::DASH)
		matWorld = matScale * matRotZ * matTrans;
	else// if (m_eCurID == EFFECT::WAVE)
		matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CEffect::Release_Object()
{
}

void CEffect::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
		m_bDead = true;
}


void CEffect::Scene_Change()
{
	if (m_ePreID != m_eCurID)
	{
		switch (m_eCurID)
		{
		case EFFECT::WAVE:
			m_tInfo.vSize = { 2.f, 2.f, 0.f };
			m_wstrObjectKey = L"BOSS_A";
			m_wstrStateKey = L"WAVE";
			m_tFrame = { 0.0, 15.0 };
			m_dFrameSpeed = 0.5;
			break;
		case EFFECT::DASH:
			m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
			m_wstrObjectKey = L"BOSS_D";
			m_wstrStateKey = L"BOSS_D_DASH";
			m_tFrame = { 0.0, 28.0 };
			m_dFrameSpeed = 2.0;
			break;
		case EFFECT::ROCK_EFFECT:
			m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
			m_wstrObjectKey = L"BOSS_A";
			m_wstrStateKey = L"ROCK_EFFECT";
			m_tFrame = { 5.0, 8.0 };
			m_dFrameSpeed = 1.0;
			break;
		}

		m_ePreID = m_eCurID;
	}
}
