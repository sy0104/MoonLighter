#pragma once

#ifndef __ELECTRICPART_H__
#define __ELECTRICPART_H__

#include "Obj.h"
class CElectricPart : public CObj
{
public:
	explicit CElectricPart();
	virtual ~CElectricPart();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	int			m_iAlpha;
};


#endif