#pragma once

#ifndef __PLANTHEAD_H__
#define __PLANTHEAD_H__

#include "Obj.h"
class CPlantHead : public CObj
{
public:
	explicit CPlantHead();
	virtual ~CPlantHead();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_Head(PLANT::HEAD eState) { m_eCurScene = eState; }
	void Set_Move() { m_bMove = true; }
	void Set_Stop() { m_bMove = false; }
	void Set_DeathMotion() { m_bDeathMotion = true; }

private:
	void Scene_Change();

// Attack
public:
	void Create_Particle();		// Particle
	void Jump_Attack();			// Jump
	void Create_Bullet();		// Shake

private:
	PLANT::HEAD		m_ePreScene;
	PLANT::HEAD		m_eCurScene;

	wstring			m_wstrStateKey;
	double			m_dFrameSpeed;

	TCHAR			m_szPos[32] = L"";
	TCHAR			m_szHp[32] = L"";

	CObj*			m_pBossHp;
	CObj*			m_pBossHpBar;

	CObj*			m_pTarget;

	bool			m_bMove;
	bool			m_bJump;
	bool			m_bSetDir;
	bool			m_bInitFrame;
	bool			m_bDeathMotion;

	// PARTICLE
	bool			m_bCreateSeed;

	// SHAKE
	float			m_fAngle1;
	float			m_fAngle2;
	float			m_fAngle3;

	double			m_dBulletTime;
	double			m_dHitTime;
};


#endif