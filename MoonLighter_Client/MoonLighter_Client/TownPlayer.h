#pragma once

#ifndef __TOWNPLAYER_H__
#define __TOWNPLAYER_H__

class CShowcase;

#include "Obj.h"
class CTownPlayer : public CObj
{
public:
	explicit CTownPlayer();
	virtual ~CTownPlayer();

public:
	enum STATE { IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN, 
				 WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN, STATE_END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	DIR::DIR Get_Dir() { return m_ePlayerDir; }
	CObj* Get_Inven() { return m_pInven; }
	int Get_Gold() { return m_iGold; }

public:
	void Set_Stop() { m_bStop = true; }
	void Set_Move() { m_bStop = false; }

private:
	void Scene_Change();
	void Key_Check();

private:
	void Open_Shop();


private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	DIR::DIR	m_ePlayerDir;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;

	TCHAR		m_szPos[32] = L"";

	CObj*		m_pInven;
	bool		m_bCreateInven;

	bool		m_bOpenShop;
	bool		m_bStop;
	double		m_bStopTime;

	double		m_dNpcTime;
	bool		m_bCreateNpc;

	vector<CObj*>	m_vecShowcase;
	bool			m_bShowcase;
};	


#endif