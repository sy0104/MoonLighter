#pragma once

#ifndef __PARTICLEBODY_H__
#define __PARTICLEBODY_H__

#include "Obj.h"
class CParticleBody : public CObj
{
public:
	explicit CParticleBody();
	virtual ~CParticleBody();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

	
private:
	double		m_dHitTime;
};


#endif