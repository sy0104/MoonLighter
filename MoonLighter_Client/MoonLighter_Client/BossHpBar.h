#pragma once

#ifndef __BOSSHPBAR_H__
#define __BOSSHPBAR_H__

#include "Obj.h"
class CBossHpBar : public CObj
{
public:
	explicit CBossHpBar();
	virtual ~CBossHpBar();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
};


#endif