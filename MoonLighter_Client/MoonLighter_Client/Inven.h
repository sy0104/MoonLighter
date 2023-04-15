#pragma once

#ifndef __INVEN_H__
#define __INVEN_H__

class CItem;

#include "Obj.h"
class CInven : public CObj
{
public:
	explicit CInven();
	virtual ~CInven();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	CObj* Get_InvenItem(int iIdx) { return m_vecInven[iIdx]; }
	vector<CObj*> Get_Table() { return m_vecTable; }

public:
	void Set_Render() { m_iAlpha = 255; }
	void Set_Hide() { m_iAlpha = 0; }

public:
	void Insert_Item(CObj* _pItem);
	void Insert_Table(CObj* _pTable);

private:
	void Sell_Item();
	void Key_Check();

private:
	vector<CObj*>	m_vecInven;
	CObj*			m_pItem;

	CObj*			m_pInvenUI;
	CObj*			m_pShowcase;

	vector<CObj*>	m_vecShowcase;
	bool			m_bShowcase;
	int				m_iShowcaseIdx;

	vector<CObj*>	m_vecTable;
	CObj*			m_pSellButton;

	wstring			m_wstrStateKey;
	int				m_iItemNum;

	bool			m_bRender;

	TCHAR			m_szItemCnt[32] = L"";

	TCHAR			m_szPlayerCoin[32] = L"";
};


#endif