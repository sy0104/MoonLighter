#pragma once

#ifndef __PUNCH_H__
#define __PUNCH_H__

#include "Obj.h"
class CPunch : public CObj
{
public:
	explicit CPunch();
	virtual ~CPunch();

public:
	enum STATE { DOWN, UP, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_PosY(float fY) { m_fPosY = fY; }

public:
	const bool& Get_Up() const { return m_bUp; }

private:
	void Scene_Change();

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	double		m_dPauseTime;
	double		m_dFrameSpeed;

	float		m_fPosY;

	bool		m_bUp;
};


#endif