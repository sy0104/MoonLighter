#include "stdafx.h"
#include "Root.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

#include "SoundMgr.h"

CRoot::CRoot()
	: m_wstrStateKey(L""), m_iCount(0), m_iRandCnt(0)
{
}


CRoot::~CRoot()
{
	Release_Object();
}

HRESULT CRoot::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_iRandCnt = rand() % 2;

	m_tUnit = { 5, 0, 0 };

	if (m_iRandCnt == 0)
	{
		m_wstrStateKey = L"ROOT_A";
		m_tFrame = { 0.0, 59.0 };
	}

	else
	{
		m_wstrStateKey = L"ROOT_B";
		m_tFrame = { 0.0, 60.0 };
	}

	CSoundMgr::Get_Instance()->PlaySound(L"Plant_StemUp.wav", CSoundMgr::EFFECT);

	return S_OK;
}

int CRoot::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	Frame_Move(0.5);

	return OBJ_NOEVENT;
}

void CRoot::LateUpdate_Object()
{
}

void CRoot::Render_Object()
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

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.5f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.5f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY));


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

void CRoot::Release_Object()
{
}

void CRoot::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
		m_bDead = true;
}
