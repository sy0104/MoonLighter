#pragma once

#ifndef __ROOT_H__
#define __ROOT_H__

#include "Obj.h"
class CRoot : public CObj
{
public:
	explicit CRoot();
	virtual ~CRoot();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);


private:
	wstring		m_wstrStateKey;

	int			m_iCount;
	int			m_iRandCnt;
};


#endif