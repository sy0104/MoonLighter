#pragma once

#ifndef __ELECTRICROOM_H__
#define __ELECTRICROOM_H__

#include "Obj.h"
class CElectricRoom : public CObj
{
public:
	explicit CElectricRoom();
	virtual ~CElectricRoom();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);


private:
	bool	m_bDeadMotion;
};


#endif