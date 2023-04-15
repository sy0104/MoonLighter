#pragma once

#ifndef __PARTICLESEED_H__
#define __PARTICLESEED_H__

#include "Obj.h"
class CParticleSeed : public CObj
{
public:
	explicit CParticleSeed();
	virtual ~CParticleSeed();

public:
	enum DIR { LEFT, RIGHT, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_PosY(float fY) { m_fPosY = fY; }

private:
	DIR			m_eDir;

	double		m_dFlyTime;
	double		m_dFallTime;

	int			m_iRand;

	float		m_fSpeedX;
	float		m_fSpeedY;

	float		m_fPosY;
};


#endif