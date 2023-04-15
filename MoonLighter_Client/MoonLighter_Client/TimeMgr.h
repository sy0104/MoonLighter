#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	HRESULT Ready_TimeMgr();
	void Update_TimeMgr();

public:
	double Get_DeltaTime() { return m_dDeltaTime; }

private:
	LARGE_INTEGER	m_CpuTick;
	LARGE_INTEGER	m_PreTime;
	LARGE_INTEGER	m_CurTime;
	double			m_dDeltaTime;
};

