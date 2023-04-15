#pragma once

#ifndef __BOW_H__
#define __BOW_H__

#include "Obj.h"
class CBow : public CObj
{
public:
	explicit CBow();
	virtual ~CBow();

public:
	enum STATE { LEFT, RIGHT, UP, DOWN, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	void Scene_Change();

public:
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }


private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	bool		m_bCreateArrow;
};


#endif