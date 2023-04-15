#pragma once
class CFrameMgr
{
public:
	CFrameMgr();
	~CFrameMgr();

public:
	void Ready_FrameMgr(const double& dFramePerSecond);
	void Render_FrameMgr();
	bool Lock_FrameMgr();

private:
	// 프레임 제한
	LARGE_INTEGER	m_CpuTick;
	LARGE_INTEGER	m_BeginTime;
	LARGE_INTEGER	m_EndTime;

	double			m_dDeltaTime;
	double			m_dSecondPerFrame;	// 1 / 60 쪼갠값을 가진 변수

										// 프레임 출력
	int				m_iFPS;
	TCHAR			m_szFPS[32];
	double			m_dFPSTime;
};

