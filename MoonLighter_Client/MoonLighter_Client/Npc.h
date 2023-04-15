#pragma once

#ifndef __NPC_H__
#define __NPC_H__

#include "Obj.h"
class CNpc : public CObj
{
public:
	explicit CNpc();
	virtual ~CNpc();

public:
	enum STATE { IDLE_LEFT, IDLE_RIGHT, IDLE_UP, IDLE_DOWN,
				 WALK_LEFT, WALK_RIGHT, WALK_UP, WALK_DOWN, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	void Set_ObjectKey(wstring wstrObjectKey) { m_wstrObjectKey = wstrObjectKey; }
	void Set_StateKey(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_Frame(double dFrameEnd) { m_tFrame = { 0.0, dFrameEnd }; }
	void Set_Walk() { m_bWalk = true; }

public:
	void Insert_Table(CObj* _pTable);

private:
	void Scene_Change();
	bool Reaction(int iIdx);

public:
	void Nathan_Walk();
	void Sakura_Walk();
	void Worker_Walk();

public:
	void Nathan_Shop();
	void Sakura_Shop();
	void Worker_Shop();
	void Rise_Shop();


private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;

	CObj*		m_pInven;

	vector<CObj*>	m_vecTable;

	bool		m_bWalk;
	bool		m_bEnter;
	bool		m_bBuy;

	double		m_dNathanTime;
	bool		m_bNathanBreak;
	bool		m_bNathanUp;
	bool		m_bNathanReaction;
	bool		m_bNathanBuy;
	bool		m_bNathanGold;

	double		m_dSakuraTime;
	bool		m_bSakuraBreak;
	bool		m_bSakuraLeft;
	bool		m_bSakuraReaction;
	bool		m_bSakuraBuy;
	bool		m_bSakuraGold;

	double		m_dWorkerTime;
	bool		m_bWorkerBreak;
	bool		m_bWorkerDown;
	bool		m_bWorkerReaction;
	bool		m_bWorkerBuy;
	bool		m_bWorkerGold;

	double		m_dRiseTime;
	bool		m_bRiseReaction;
	bool		m_bRiseBuy;
	bool		m_bRiseGold;

};


#endif