#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "Obj.h"
class CItem : public CObj
{
public:
	explicit CItem();
	virtual ~CItem();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	const wstring& Get_StateKey() const { return m_wstrStateKey; }
	const int& Get_ItemNum() const { return m_iItemNum; }
	const int& Get_ItemCount() const { return m_iCount; }
	int& Get_InputPrice() { return m_iInputPrice; }
	int& Get_Price() { return m_iPrice; }

public:
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_ItemNum(int iNum) { m_iItemNum = iNum; }
	void Set_ItemCount() { ++m_iCount; }
	void Set_ItemCountMinus() { --m_iCount; }
	void Set_InputPrice(int iInput);
	void Set_Price(int iPrice) { m_iPrice = iPrice; }

private:
	wstring		m_wstrStateKey;
	int			m_iItemNum;
	int			m_iCount;

	int			m_iPrice;
	int			m_iInputPrice;

	bool		m_bCreated;
};



#endif