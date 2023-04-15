#pragma once

#ifndef __SLIME_H__
#define __SLIME_H__

#include "Obj.h"
class CSlime : public CObj
{
public:
	explicit CSlime();
	virtual ~CSlime();

public:
	enum STATE { IDLE, JUMP, DEATH, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	void Scene_Change();

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	double		m_dHitTime;
	bool		m_bInitFrame;

	RECT		rc = {};
	RECT		m_tRange;
};

#endif