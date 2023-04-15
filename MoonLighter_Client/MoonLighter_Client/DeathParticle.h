#pragma once

#ifndef __DEATHPARTICLE_H__
#define __DEATHPARTICLE_H__


#include "Obj.h"
class CDeathParticle : public CObj
{
public:
	explicit CDeathParticle();
	virtual ~CDeathParticle();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

};


#endif