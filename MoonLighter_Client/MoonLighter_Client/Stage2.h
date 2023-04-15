#pragma once

#ifndef __STAGE2_H__
#define __STAGE2_H__

#include "Scene.h"
class CStage2 : public CScene
{
public:
	explicit CStage2();
	virtual ~CStage2();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	CObj*	m_pPlayer;
	CObj*	m_pPlant;

	double	m_dSceneChangeTime;
};


#endif