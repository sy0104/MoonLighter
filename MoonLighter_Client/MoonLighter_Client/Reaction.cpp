#include "stdafx.h"
#include "Reaction.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "SpeechBubble.h"


CReaction::CReaction()
	: m_wstrStateKey(L""), m_wstrSecondStateKey(L""), m_dSecondEnd(0.0), m_pBubble(nullptr), m_bCreateBubble(false)
{
}


CReaction::~CReaction()
{
	Release_Object();
}

HRESULT CReaction::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };


	return S_OK;
}

int CReaction::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bCreateBubble)
	{
		m_pBubble = CAbstractFactory<CSpeechBubble>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUBBLE, m_pBubble);

		m_bCreateBubble = true;
	}


	Frame_Move(0.8);
	return OBJ_NOEVENT;
}

void CReaction::LateUpdate_Object()
{
}

void CReaction::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"REACTION", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CReaction::Release_Object()
{
}

void CReaction::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart > m_tFrame.dFrameEnd)
	{
		if (m_wstrStateKey == L"Think" || m_wstrStateKey == L"Alert" || m_wstrStateKey == L"Doubt")
		{
			m_wstrStateKey = m_wstrSecondStateKey;
			m_tFrame = { 0.0, m_dSecondEnd };
		}
		else
		{
			m_pBubble->Set_Dead();
			m_bDead = true;
		}
	}
}

void CReaction::Scene_Change()
{
	if (m_wstrStateKey == L"Alert")
		m_tFrame = { 0.0, 6.0 };

	else if (m_wstrStateKey == L"Angry")
		m_tFrame = { 0.0, 2.0 };

	else if (m_wstrStateKey == L"Cheap")
		m_tFrame = { 0.0, 1.0 };

	else if (m_wstrStateKey == L"Doubt")
		m_tFrame = { 0.0, 18.0 };

	else if (m_wstrStateKey == L"Expensive")
		m_tFrame = { 0.0, 2.0 };

	else if (m_wstrStateKey == L"Think")
		m_tFrame = { 0.0, 3.0 };

	else if (m_wstrStateKey == L"TooCheap")
		m_tFrame = { 0.0, 10.0 };

	else if (m_wstrStateKey == L"TooExpensive")
		m_tFrame = { 0.0, 6.0 };

	else if (m_wstrStateKey == L"Transition")
		m_tFrame = { 0.0, 2.0 };
}
