#pragma once

#ifndef __BOSSHP_H__
#define __BOSSHP_H__

#include "Obj.h"
class CBossHp : public CObj
{
public:
	explicit CBossHp();
	virtual ~CBossHp();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_HpRect(int iRight) { m_tHpRect.right = iRight; }

private:
	RECT	m_tHpRect;
};


#endif