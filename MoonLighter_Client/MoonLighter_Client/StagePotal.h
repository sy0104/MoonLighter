#pragma once

#ifndef __STAGEPOTAL_H__
#define __STAGEPOTAL_H__

#include "Obj.h"
class CStagePotal : public CObj
{
public:
	explicit CStagePotal();
	virtual ~CStagePotal();

public:
	enum STATE { IDLE, OPEN, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_Open() { m_bOpen = true; }


private:
	bool		m_bOpen;
};


#endif
