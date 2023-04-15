#pragma once

#ifndef __POTAL_H__
#define __POTAL_H__

#include "Obj.h"
class CPotal : public CObj
{
public:
	explicit CPotal();
	virtual ~CPotal();

public:
	enum STATE { IDLE, OPEN, CLOSE, END };

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
	void Set_Key(wstring OKey, wstring SKey) { m_wstrObjectKey = OKey; m_wstrStateKey = SKey; }
	void Set_Frame(double dStart, double dEnd) { m_tFrame = { dStart, dEnd }; }

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;
};


#endif