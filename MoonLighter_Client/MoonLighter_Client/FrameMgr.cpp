#include "stdafx.h"
#include "FrameMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"


CFrameMgr::CFrameMgr()
	: m_dDeltaTime(0.0), m_dSecondPerFrame(0.0), m_iFPS(0)
	, m_szFPS(L""), m_dFPSTime(0.0)
{
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));
}


CFrameMgr::~CFrameMgr()
{
}

void CFrameMgr::Ready_FrameMgr(const double& dFramePerSecond)
{
	m_dSecondPerFrame = 1.0 / dFramePerSecond;

	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	QueryPerformanceCounter(&m_BeginTime);
}

void CFrameMgr::Render_FrameMgr()
{
	m_dFPSTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_dFPSTime > 1.0)
	{
	//	swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		m_dFPSTime = 0.0;
		m_iFPS = 0;
	}

	D3DXMATRIX	matTrans;

	D3DXMatrixTranslation(&matTrans, 1100.f, 50.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szFPS, lstrlen(m_szFPS), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool CFrameMgr::Lock_FrameMgr()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);

	m_dDeltaTime += double(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;
	m_BeginTime = m_EndTime;

	if (m_dSecondPerFrame < m_dDeltaTime)
	{
		m_dDeltaTime = 0.0;
		++m_iFPS;
		return true;
	}

	return false;
}
