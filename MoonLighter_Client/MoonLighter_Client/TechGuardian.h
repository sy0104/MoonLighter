#pragma once

#ifndef __CTECHGUARDIAN_H__
#define __CTECHGUARDIAN_H__

#include "Obj.h"
class CTechGuardian : public CObj
{
public:
	explicit CTechGuardian();
	virtual ~CTechGuardian();

public:
	enum STATE { WAKEUP, WAKEUP_IDLE, IDLE, TELEPORT, BALL_ATTACK, ELECTRIC_PART, DASH, DEATH, END };
	
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
	void Progress();

	void Teleport(float fX, float fY);
	void Ball_Attack();
	void Dash();
	void Electric_Part(int iDirNum);


	
private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	TCHAR		m_szWaveTime[32] = L"";
	TCHAR		m_szPos[32] = L"";
	TCHAR		m_szHp[64] = L"";

	CObj*		m_pBossHp;
	CObj*		m_pBossHpBar;

	double		m_dFrameSpeed;
	double		m_dWaveTime;
	double		m_dHitTime;

	bool		m_bIntro;
	bool		m_bTeleport;
	bool		m_bAttack;
	bool		m_bInitFrame;
	bool		m_bInitDeath;

	// Ball Attack
	bool		m_bBallAttack;
	bool		m_bCreateBalls;
	CObj*		m_pBodyBeam;
	CObj*		m_pOutBeam;
	CObj*		m_pBeam;

	// Dash
	CObj*		m_pTarget;
	bool		m_bSetTarget;
	bool		m_bDash;
	bool		m_bDashEffect;
	int			m_iDashCnt;

	// Electric Part
	bool		m_bCreateCircle;
	bool		m_bCreatePart;
	list<CObj*>	m_listPart;
	float		m_fPartAngle;
	float		m_fPartDis;
	double		m_dPartTime;
};


#endif