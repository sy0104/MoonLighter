#pragma once

#ifndef __ARROW_H__
#define __ARROW_H__

#include "Obj.h"
class CArrow : public CObj
{
public:
	explicit CArrow();
	virtual ~CArrow();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

private:
	void Scene_Change();

public:
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_Dir(DIR::DIR eDir) { m_eDir = eDir; }

private:
	DIR::DIR	m_ePreScene;
	DIR::DIR	m_eCurScene;

	DIR::DIR	m_eDir;

	wstring		m_wstrStateKey;
};


#endif