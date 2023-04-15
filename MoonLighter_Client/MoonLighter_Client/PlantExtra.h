#pragma once

#ifndef __PLANTEXTRA_H__
#define __PLANTEXTRA_H__

#include "Obj.h"
class CPlantExtra : public CObj
{
public:
	explicit CPlantExtra();
	virtual ~CPlantExtra();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_Frame(double dFrameEnd) { m_tFrame = { 0.0, dFrameEnd }; }

private:
	wstring		m_wstrStateKey;
};


#endif