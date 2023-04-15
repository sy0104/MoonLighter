#pragma once

#ifndef __SHOPTABLE_H__
#define __SHOPTABLE_H__

#include "Obj.h"
class CShopTable : public CObj
{
public:
	explicit CShopTable();
	virtual ~CShopTable();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	int Get_OriginalPrice();
	int Get_InputPrice();

public:
	void Render_Item() { m_iItemAlpha = 255; }
	void Hide_Item() { m_iItemAlpha = 0; }

public:
	void Set_Item(CObj* pItem) { m_pItem = pItem; }

public:
	CObj* Get_Item() { return m_pItem; }

private:
	CObj*		m_pItem;
	TCHAR		m_szPrice[32] = L"";
	TCHAR		m_szOrigin[32] = L"";

	int			m_iItemAlpha;
};


#endif