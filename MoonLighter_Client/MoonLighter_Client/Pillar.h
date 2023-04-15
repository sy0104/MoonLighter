#pragma once

#ifndef __PILLAR_H__
#define __PILLAR_H__

#include "Obj.h"
class CPillar : public CObj
{
public:
	explicit CPillar();
	virtual ~CPillar();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	bool Get_Check() { return m_bCheck; }

public:
	void Set_CheckTrue() { m_bCheck = true; }
	void Set_CheckFalse() { m_bCheck = false; }
	void Set_ExtendY() { m_bExtendY = true; }
	void Set_ExtendX() { m_bExtendX = true; }

private:
	bool		m_bCheck;
	bool		m_bExtendY;
	bool		m_bExtendX;

};


#endif