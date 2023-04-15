#include "stdafx.h"
#include "ElectricPart.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

#include "SoundMgr.h"

CElectricPart::CElectricPart()
	: m_iAlpha(255)
{
}


CElectricPart::~CElectricPart()
{
	Release_Object();
}

HRESULT CElectricPart::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_tFrame = { (double)(rand() % 10), 10.0 };

	m_fSpeed = 3.f;
	m_fAngle = (float)(rand() % 360);

	//CSoundMgr::Get_Instance()->PlaySound(L"Tech_Electric.wav", CSoundMgr::EFFECT);


	return S_OK;
}

int CElectricPart::Update_Object()
{
	if (m_bDead)
		m_iAlpha -= 1;

	if (m_iAlpha <= 0)
		return OBJ_DEAD;

	CSoundMgr::Get_Instance()->PlaySound(L"Tech_Ball.wav", CSoundMgr::EFFECT);

	Frame_Move();

	return OBJ_NOEVENT;
}

void CElectricPart::LateUpdate_Object()
{
}

void CElectricPart::Render_Object()
{
	D3DXMATRIX	matScale, matRotZ, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_D", L"BOSS_D_ELECTIC_PART", int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matRotZ * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.2f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.2f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.2f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.2f));


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

void CElectricPart::Release_Object()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
}

void CElectricPart::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		m_tFrame.dFrameStart = 0.0;
	}

}
