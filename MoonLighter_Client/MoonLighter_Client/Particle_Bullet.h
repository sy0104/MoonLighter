#pragma once

#ifndef __PARTICLEBULLET_H__
#define __PARTICLEBULLET_H__

#include "Obj.h"
class CParticle_Bullet : public CObj
{
public:
	explicit CParticle_Bullet();
	virtual ~CParticle_Bullet();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_Guide() { m_bGuide = true; }

private:
	bool	m_bDeathMotion;

	bool	m_bGuide;
	bool	m_bCheck;
};


#endif