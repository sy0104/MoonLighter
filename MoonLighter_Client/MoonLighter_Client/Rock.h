#pragma once

#ifndef __ROCK_H__
#define __ROCK_H__

#include "Obj.h"
class CRock : public CObj
{
public:
	explicit CRock();
	virtual ~CRock();


public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;


public:
	void Set_PosY(float _y) { m_fPosY = _y; }

private:
	bool		m_bChange;

	int			m_iRockNum;

	float		m_fPosY;

	double		m_dShadowTime;
	double		m_dRockTime;

	bool		m_bSound;
	double		m_dSoundTime;
};


#endif