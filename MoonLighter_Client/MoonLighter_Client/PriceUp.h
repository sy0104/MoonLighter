#pragma once

#ifndef __PRICEUP_H__
#define __PRICEUP_H__

#include "Obj.h"
class CPriceUp : public CObj
{
public:
	explicit CPriceUp();
	virtual ~CPriceUp();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_Render() { m_iAlpha = 255; }
	void Set_Hide() { m_iAlpha = 0; }
};


#endif