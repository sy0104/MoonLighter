#pragma once
#include "Obj.h"


class CInvenUI : public CObj
{
public:
	explicit CInvenUI();
	virtual ~CInvenUI();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_Render() { m_iAlpha = 255; }
	void Set_Hide() { m_iAlpha = 0; }

private:
	void Key_Check();

private:
	TCHAR	m_szPos[32] = L"";
};

