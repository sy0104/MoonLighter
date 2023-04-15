#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Obj.h"

class CPlayer : public CObj
{
public:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	enum STATE {
		IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
		WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN,
		ROLL_LEFT, ROLL_RIGHT, ROLL_UP, ROLL_DOWN,
		ATTACK_LEFT, ATTACK_RIGHT, ATTACK_UP, ATTACK_DOWN,
		HIT_LEFT, HIT_RIGHT, HIT_UP, HIT_DOWN, STATE_END
	};


public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	const DIR::DIR& Get_Dir() const { return m_ePlayerDir; }
	const WEAPON::ID& Get_WeaponID() const { return m_eWeaponState; }
	CObj* Get_Weapon();
	int Get_Gold() { return m_iGold; }

public:
	void Set_Hit() { m_bHit = true; }
	void Set_Stop() { m_bStop = true; }
	void Set_Move() { m_bStop = false; }

private:
	void KeyCheck();
	void OffSet();
	void Scene_Change();


private:

	STATE		m_ePreScene;
	STATE		m_eCurScene;
	DIR::DIR	m_ePlayerDir;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;

	WEAPON::ID	m_eWeaponState;
	CObj*		m_pWeapon;

	CObj*		m_pPlayerHp;
	CObj*		m_pPlayerHpBar;

	CObj*		m_pInven;
	bool		m_bCreateInven;

	bool		m_bRoll;
	bool		m_bAttack;
	bool		m_bHit;
	bool		m_bStop;
	bool		m_bCreateWeapon;

	double		m_dCollisionBreak;
	double		m_dFrameSpeed;

	TCHAR		m_szPos[64] = L"";
	TCHAR		m_szHp[32] = L"";

	bool		m_bBreak;
};


#endif