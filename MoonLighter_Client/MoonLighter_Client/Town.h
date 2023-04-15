#pragma once

#ifndef __TOWN_H__
#define __TOWN_H__

#include "Scene.h"
class CTown : public CScene
{
public:
	explicit CTown();
	virtual ~CTown();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


private:
	CObj*	m_pPlayer;

	CObj*	m_pNathan;
	CObj*	m_pSakura;
	CObj*	m_pWorker;

	bool	m_bBgm;
};


#endif