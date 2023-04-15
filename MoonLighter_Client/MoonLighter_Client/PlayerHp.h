#pragma once

#ifndef __PLAYERHP_H__
#define __PLAYERHP_H__


#include "Obj.h"
class CPlayerHp : public CObj
{
public:
	explicit CPlayerHp();
	virtual ~CPlayerHp();

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