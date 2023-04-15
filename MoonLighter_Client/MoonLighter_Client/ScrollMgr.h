#pragma once

#ifndef __SCROLLMGR_H__
#define __SCROLLMGR_H__

class CScrollMgr
{
	DECLARE_SINGLETON(CScrollMgr)

public:
	enum SCROLL { SCROLL_X, SCROLL_Y };

private:
	CScrollMgr();
	~CScrollMgr();

public:
	void Set_Scroll(const D3DXVECTOR3& vScroll) { m_vScroll += vScroll; }
	void Set_ScrollX(float fScrollX) { m_vScroll.x += fScrollX; }
	void Set_ScrollY(float fScrollY) { m_vScroll.y += fScrollY; }
	void Set_ScrollLockX(float fLockX) { m_fLockX = fLockX; }
	void Set_ScrollLockY(float fLockY) { m_fLockY = fLockY; }

public:
	const D3DXVECTOR3& Get_Scroll() { return m_vScroll; }
	const float Get_Scroll(SCROLL eScrollID);

public:
	void Scroll_Lock();

private:
	D3DXVECTOR3	m_vScroll;
	float	m_fLockX;
	float	m_fLockY;
};


#endif