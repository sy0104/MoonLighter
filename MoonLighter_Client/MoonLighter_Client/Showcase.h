#pragma once

#ifndef __SHOWCASE_H__
#define __SHOWCASE_H__

class CItem;

#include "Obj.h"
class CShowcase : public CObj
{
public:
	explicit CShowcase();
	virtual ~CShowcase();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	CObj* Get_Item() { return m_pItem; }
	wstring Get_StateKey();
	int Get_ItemNum();
	int Get_InputPrice();
	int Get_OriginalPrice();

public:
	void Set_Item(CObj* pItem) { m_pItem = pItem; }
	void Set_Price(int iPrice);
	void Set_Render() { m_iAlpha = 255; }
	void Set_Hide() { m_iAlpha = 0; }

public:
	void Delete_Item() { SAFE_DELETE(m_pItem); }

private:
	void Key_Check();

private:
	CObj*		m_pItem;
	TCHAR		m_szPrice[32] = L"";

	bool		m_bCreateButton;
	CObj*		m_pPriceUp;
	CObj*		m_pPriceDown;
};

#endif