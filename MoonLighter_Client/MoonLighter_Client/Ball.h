#pragma once

#ifndef __BALL_H__
#define __BALL_H__

#include "Obj.h"
class CBall : public CObj
{
public:
	explicit CBall();
	virtual ~CBall();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

private:
	CObj*	m_pTarget;
	bool	m_bCheck;

	int		m_iAlpha;

};


#endif