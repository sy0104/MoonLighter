#pragma once

#ifndef __EXTRAPARTICLE_H__
#define __EXTRAPARTICLE_H__

#include "Obj.h"
class CExtraParticle : public CObj
{
public:
	explicit CExtraParticle();
	virtual ~CExtraParticle();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	PARTICLE::EXTRA		m_eCurScene;

	wstring				m_wstrStateKey;
};


#endif