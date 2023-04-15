#pragma once

#ifndef __BODYBEAM_H__
#define __BODYBEAM_H__

#include "Obj.h"
class CBeam : public CObj
{
public:
	explicit CBeam();
	virtual ~CBeam();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;


private:
	int		m_iAlpha;
	bool	m_bCreate;
};


#endif