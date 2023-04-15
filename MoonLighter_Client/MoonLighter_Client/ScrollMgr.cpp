#include "stdafx.h"
#include "ScrollMgr.h"

IMPLEMENT_SINGLETON(CScrollMgr)

CScrollMgr::CScrollMgr()
	: m_fLockX(-600.f), m_fLockY(-400.f)
{
}


CScrollMgr::~CScrollMgr()
{
}

const float CScrollMgr::Get_Scroll(SCROLL eScrollID)
{
	switch (eScrollID)
	{
	case CScrollMgr::SCROLL_X:
		return m_vScroll.x;
	case CScrollMgr::SCROLL_Y:
		return m_vScroll.y;
	default:
		return 0.f;
	}

	return 0.0f;
}

void CScrollMgr::Scroll_Lock()
{
	if (m_vScroll.x > 0)
		m_vScroll.x = 0.f;

	if (m_vScroll.x < m_fLockX)
		m_vScroll.x = m_fLockX;

	if (m_vScroll.y > 0)
		m_vScroll.y = 0.f;

	if (m_vScroll.y < m_fLockY)
		m_vScroll.y = m_fLockY;

		
}
