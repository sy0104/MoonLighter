#pragma once

#ifndef __PLAYERHPBAR_H__
#define __PLAYERHPBAR_H__

#include "Obj.h"
class CPlayerHpBar : public CObj
{
public:
	explicit CPlayerHpBar();
	virtual ~CPlayerHpBar();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
};


#endif