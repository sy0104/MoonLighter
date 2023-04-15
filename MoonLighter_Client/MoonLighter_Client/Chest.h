#pragma once

#ifndef __CHEST_H__
#define __CHEST_H__

#include "Obj.h"
class CChest : public CObj
{
public:
	explicit CChest();
	virtual ~CChest();

public:
	enum STATE { IDLE, OPEN, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_Open() { m_bOpen = true; }
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_Frame(double dFrameEnd) { m_tFrame = { 0.0, dFrameEnd }; }

private:
	void Scene_Change();

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrStateKey;

	bool		m_bStart;
	bool		m_bOpen;
	bool		m_bDrop;
};


#endif