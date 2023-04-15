#pragma once

#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "Obj.h"
class CShadow : public CObj
{
public:
	explicit CShadow();
	virtual ~CShadow();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	// 0: Rock Shaodw, 1: Punch Shadow
	void Set_Shadow(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_RandomRock() { m_bRandomRock = true; }

private:
 	wstring		m_wstrStateKey;

	int			m_iKind;

	double		m_dDeathTime;
	double		m_dMoveTime;
	double		m_dPauseTime;

	bool		m_bRandomRock;
	bool		m_bCreateRandomRock;
	bool		m_bCreatePunch;

	bool		m_bMove;

	CObj*		m_pPunch;
};


#endif