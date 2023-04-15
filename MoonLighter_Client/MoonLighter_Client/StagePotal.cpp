#include "stdafx.h"
#include "StagePotal.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

CStagePotal::CStagePotal()
	: m_bOpen(false)
{
}


CStagePotal::~CStagePotal()
{
	Release_Object();
}

HRESULT CStagePotal::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 3.0 };

	return S_OK;
}

int CStagePotal::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;


	if (m_bOpen)
		Frame_Move();


	return OBJ_NOEVENT;
}

void CStagePotal::LateUpdate_Object()
{
	RECT rc = {};

	RECT rPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_Rect();

	if (m_bOpen && IntersectRect(&rc, &m_tRect, &rPlayer))
	{
		CSceneMgr::Get_Instance()->Change_Scene(CSceneMgr::STAGE1);
		return;
	}
}

void CStagePotal::Render_Object()
{
	// POTAL_STAGE
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"POTAL_STAGE", L"IDLE", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 1.f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 1.f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 1.f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.5f));

#ifdef _DEBUG
	D3DXVECTOR2 vLine[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

#endif

}

void CStagePotal::Release_Object()
{
}

void CStagePotal::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;


	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_tFrame.dFrameStart = m_tFrame.dFrameEnd;
	}
}
