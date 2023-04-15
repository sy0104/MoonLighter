#include "stdafx.h"
#include "Punch.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

#include "SoundMgr.h"

CPunch::CPunch()
	: m_ePreScene(END), m_eCurScene(END), m_wstrStateKey(L""), m_fPosY(0.f), m_dPauseTime(0.0), m_dFrameSpeed(1.0)
	, m_bUp(false)
{
}


CPunch::~CPunch()
{
	Release_Object();
}

HRESULT CPunch::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
		// 초기값 Shadow 좌표로 설정
//	m_tInfo.vPos = { CObjMgr::Get_Instance()->Get_Object(OBJ::)}

	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 5.f;

	m_ePreScene = DOWN;
	m_eCurScene = DOWN;
	m_wstrStateKey = L"PUNCH_DOWN";
	m_tFrame = { 0.0, 9.0 };


	return S_OK;
}

int CPunch::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_eCurScene == DOWN && m_tInfo.vPos.y < m_fPosY)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Golem_PunchDown.wav", CSoundMgr::EFFECT);
		m_tInfo.vPos.y += m_tInfo.vDir.y * 40.f;
		m_tFrame.dFrameStart = 0.0;
	}

	else if (m_eCurScene == UP && m_tInfo.vPos.y > -60.f)
	{
		if (m_tFrame.dFrameStart > 5.0)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Golem_Punch.wav", CSoundMgr::EFFECT);
			m_tInfo.vPos.y -= m_tInfo.vDir.y * 50.f;
			m_bUp = true;
		}
	}

	else if (m_tInfo.vPos.y <= -60.f)
		m_bDead = true;

	Frame_Move(m_dFrameSpeed);
	Scene_Change();

	return OBJ_NOEVENT;
}

void CPunch::LateUpdate_Object()
{
}

void CPunch::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_A", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

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

void CPunch::Release_Object()
{
//	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
}

void CPunch::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	// 충돌체크 할 타이밍
	if (m_eCurScene == DOWN)
	{
		if (m_tFrame.dFrameStart >= 5)
			m_bCollision = true;
	}

	else if (m_eCurScene == UP)
	{
		if (m_tFrame.dFrameStart >= 5)
			m_bCollision = false;
	}


	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == DOWN)
			m_eCurScene = UP;

		m_tFrame.dFrameStart = 0.0;
	}
}

void CPunch::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case CPunch::DOWN:
			m_wstrStateKey = L"PUNCH_DOWN";
			m_tFrame = { 0.0, 9.0 };
			m_dFrameSpeed = 1.0;
			break;
		case CPunch::UP:
			m_wstrStateKey = L"PUNCH_UP";
			m_tFrame = { 0.0, 9.0 };
			m_dFrameSpeed = 2.0;
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
