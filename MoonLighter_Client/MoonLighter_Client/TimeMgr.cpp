#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
}


CTimeMgr::~CTimeMgr()
{
}

HRESULT CTimeMgr::Ready_TimeMgr()
{
	m_dDeltaTime = 0.0;

	QueryPerformanceFrequency(&m_CpuTick);

	QueryPerformanceCounter(&m_PreTime);
	QueryPerformanceCounter(&m_CurTime);

	return S_OK;
}

void CTimeMgr::Update_TimeMgr()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_CurTime);

	m_dDeltaTime = double(m_CurTime.QuadPart - m_PreTime.QuadPart) / m_CpuTick.QuadPart;

	m_PreTime = m_CurTime;
}
