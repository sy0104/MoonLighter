#pragma once

#ifndef __REACTION_H__
#define __REACTION_H__

#include "Obj.h"
class CReaction : public CObj
{
public:
	explicit CReaction();
	virtual ~CReaction();

public:
	enum STATE { ALERT, ANGRY, CHEAP, DOUBT, EXPENSIVE, THINK, TOO_CHEAP, TOO_EXPENSIVE, TRANSITION, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

private:
	void Scene_Change();

public:
	void Set_State(wstring wstrStateKey) { m_wstrStateKey = wstrStateKey; }
	void Set_SecondState(wstring wstrSecond) { m_wstrSecondStateKey = wstrSecond; }
	void Set_Frame(double dFrameEnd) { m_tFrame = { 0.0, dFrameEnd }; }
	void Set_SecondFrame(double dFrameEnd) { m_dSecondEnd = dFrameEnd; }


private:
	wstring		m_wstrStateKey;
	wstring		m_wstrSecondStateKey;

	double		m_dSecondEnd;

	CObj*		m_pBubble;
	bool		m_bCreateBubble;
};


#endif