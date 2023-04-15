#include "stdafx.h"
#include "Beam.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "Ball.h"


CBeam::CBeam()
	: m_iAlpha(255), m_bCreate(false)
{
}


CBeam::~CBeam()
{
	Release_Object();
}

HRESULT CBeam::Ready_Object()
{
	//m_tInfo.vPos.x = CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).front()->Get_Info().vPos.x;
	//m_tInfo.vPos.y = CObjMgr::Get_Instance()->Get_List(OBJ::BOSS).front()->Get_Info().vPos.y;
	//m_tInfo.vPos.z = 0.f;

	//m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.f, 30.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { 0.0, 9.0 };

	m_fAngle = 90.f;

	return S_OK;
}

int CBeam::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAngle -= 1.f;


	Frame_Move(1.5);

	return OBJ_NOEVENT;
}

void CBeam::LateUpdate_Object()
{
}

void CBeam::Render_Object()
{
	D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_D", L"BOSS_D_BEAM_BAR", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

//
//	// Rect
//	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 15.f));
//	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 2.f));
//	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 15.f));
//	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 2.f));
//
//
//#ifdef _DEBUG
//	// 충돌박스 출력
//	D3DXVECTOR2 vLine[5] =
//	{
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y }
//	};
//
//	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
//	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
//	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
//#endif
//
}

void CBeam::Release_Object()
{
}