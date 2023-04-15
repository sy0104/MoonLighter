#pragma once

#ifndef __BIGSWORD_H__
#define __BIGSWORD_H__

#include "Obj.h"
class CBigSword : public CObj
{
public:
	explicit CBigSword();
	virtual ~CBigSword();

public:
	enum STATE { LEFT, RIGHT, UP, DOWN, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);


public:
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }

private:
	void Scene_Change();

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;
};


#endif