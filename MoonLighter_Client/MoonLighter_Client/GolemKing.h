#pragma once

#ifndef __GOLEMKING_H__
#define __GOLEMKING_H__

#include "Obj.h"
class CGolemKing : public CObj
{
public:
	explicit CGolemKing();
	virtual ~CGolemKing();

public:
	enum STATE { INTRO, IDLE, SPAWN, LAUNCH, NOARM_IDLE, RECOVER, DEATH, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	void Scene_Change();
	void Initialize();

// Attack
private:
	void Progress();			// 공격 진행 함수

	void Spawn_Shadow();		// 돌 부채꼴 그림자 만들기
	void Spawn_Rock();			// 돌 부채꼴로 떨구기

	void Spawn_Rock_Random();	// 돌 랜덤하게 떨구기

	void Launch();				// 팔 떨구기
	
// Effect
	void Create_WaveEffect();

private:
	TCHAR		m_szWaveTime[32] = L"";
	TCHAR		m_szName[32] = L"";
	TCHAR		m_szHp[32] = L"";

	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	CObj*		m_pBossHp;
	CObj*		m_pBossHpBar;

	double		m_dFrameSpeed;
	double		m_dWaveTime;
	double		m_dHitTime;

	bool		m_bIntro;
	bool		m_bDeadMotion;
	bool		m_bWaveEffect;
	bool		m_bInitFrame;

	bool		m_bSpawn = false;

	// Spawn
	D3DXVECTOR2		m_vShadowPos = { 0.f, 0.f };
	D3DXVECTOR2		m_vRockPos = { 0.f, 0.f };
	bool			m_bCreateRockShadow;
	double			m_dRockTime;
	bool			m_bCreateRock1;
	bool			m_bCreateRock2;
	bool			m_bCreateRock3;
	bool			m_bCreateRandomRock;
	float			m_fRockAngle;
	float			m_fRockDis;

	// Launch
	bool			m_bLaunch;
	bool			m_bRecover;
	bool			m_bCreatePunchShadow;
	int				m_iLaunchCnt;

};



#endif