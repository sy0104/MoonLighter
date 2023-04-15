#pragma once

#ifndef __PLANTBODY_H__
#define __PLANTBODY_H__

#include "Obj.h"
class CPlantBody : public CObj
{
public:
	explicit CPlantBody();
	virtual ~CPlantBody();


public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_Body(PLANT::BODY eState) { m_eCurScene = eState; }
	void Set_Move() { m_bMove = true; }
	void Set_Stop() { m_bMove = false; }
	void Set_DeathMotion() { m_bDeathMotion = true; }

	// stem
	void Set_RootTime() { m_dRootTime = 0.0; }
	void Set_CreateRoot() { m_bCreateRoot = false; }

public:
	void Jump_Attack();		// Jump


private:
	void Scene_Change();

public:
	void Create_Root();		// STEM

private:
	PLANT::BODY		m_ePreScene;
	PLANT::BODY		m_eCurScene;

	wstring			m_wstrStateKey;

	TCHAR			m_szHp[32] = L"";

	CObj*			m_pBossHp;
	CObj*			m_pBossHpBar;

	CObj*			m_pTarget;

	bool			m_bMove;
	bool			m_bJump;
	bool			m_bSetDir;
	bool			m_bInitFrame;
	bool			m_bDeathMotion;

	double			m_dHitTime;
	double			m_dFrameSpeed = 0.5;

	// Stem
	double			m_dRootTime;
	bool			m_bCreateRoot;
};


#endif