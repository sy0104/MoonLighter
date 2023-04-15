#include "stdafx.h"
#include "Rock.h"
#include "Graphic_Device.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

#include "Effect.h"

CRock::CRock()
	: m_iRockNum(0), m_bChange(false), m_fPosY(0.f), m_dRockTime(0.0), m_bSound(false), m_dSoundTime(0.0)
{
}


CRock::~CRock()
{
	Release_Object();
}

HRESULT CRock::Ready_Object()
{
	m_tInfo.vPos = { 200.f, 400.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iRockNum = rand() % 6;

	return S_OK;
}

int CRock::Update_Object()
{
	if (m_bDead)
	{
		CObj* pObj = CAbstractFactory<CEffect>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::EFFECT, pObj);
		dynamic_cast<CEffect*>(pObj)->Set_EffectID(EFFECT::ROCK_EFFECT);

		return OBJ_DEAD;
	}

	if (m_tInfo.vPos.y < m_fPosY)
		m_tInfo.vPos.y += m_tInfo.vDir.y * 40.f;

	else
	{
		m_dSoundTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
		if (m_dSoundTime >= 0.0 && m_dSoundTime < 0.6)
			CSoundMgr::Get_Instance()->PlaySound(L"Golem_Rock.wav", CSoundMgr::EFFECT);
		else if (m_dSoundTime >= 1.0)
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		m_bCollision = true;
	}

	return OBJ_NOEVENT;
}

void CRock::LateUpdate_Object()
{
	m_dRockTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dRockTime > 5.0)
		m_bDead = true;
	
}

void CRock::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_A", L"ROCK", m_iRockNum);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.5f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));


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

void CRock::Release_Object()
{

}
