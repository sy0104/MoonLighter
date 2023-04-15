#include "stdafx.h"
#include "Obj.h"
#include "TimeMgr.h"


CObj::CObj()
	: m_fAngle(0.f), m_fSpeed(0.f), m_bDead(false), m_bCollision(false), m_bAlpha(false), m_iAlpha(255), m_iGold(1000)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
	ZeroMemory(&m_tUnit, sizeof(m_tUnit));
}


CObj::~CObj()
{
}

void CObj::Frame_Move(const double & rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd* CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
		m_tFrame.dFrameStart = 0.0;
}
