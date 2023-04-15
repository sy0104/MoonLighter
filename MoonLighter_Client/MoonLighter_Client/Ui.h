#pragma once

#ifndef __UI_H__
#define __UI_H__

#include "Obj.h"
class CUi : public CObj
{
public:
	explicit CUi();
	virtual ~CUi();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_UI(UI::ID eID) { m_eCurID = eID; }

private:
	void Scene_Change();


private:
	UI::ID		m_ePreID;
	UI::ID		m_eCurID;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;

	int			m_iCount;
};


#endif