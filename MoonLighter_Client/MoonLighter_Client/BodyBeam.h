#pragma once

#include "Obj.h"
class CBodyBeam : public CObj
{
public:
	explicit CBodyBeam();
	virtual ~CBodyBeam();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	int		m_iAlpha;
};


