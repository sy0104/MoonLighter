#pragma once

#ifndef __PRESTAGE_H__
#define __PRESTAGE_H__

#include "Scene.h"
class CPreStage : public CScene
{
public:
	explicit CPreStage();
	virtual ~CPreStage();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	void Collision_Arrow_To_Pillar(list<CObj*>& _pArrow, CObj* pPillar, int iNum);

private:
	CObj*		m_pPlayer;
	CObj*		m_pPotal;
	CObj*		m_pChest;

	CObj*		m_pPillar1;
	CObj*		m_pPillar2;
	CObj*		m_pPillar3;
	CObj*		m_pPillar4;

	double		m_dPillarTime;

	bool		m_bPillar1;
	bool		m_bPillar2;
	bool		m_bPillar3;
	bool		m_bPillar4;

	bool		m_bOpen;
	bool		m_bPotal;
};


#endif