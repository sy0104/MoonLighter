#pragma once

#ifndef __STAGE3_H__
#define __STAGE3_H__

#include "Scene.h"
class CStage3 : public CScene
{
public:
	explicit CStage3();
	virtual ~CStage3();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	CObj*	m_pPlayer;
	CObj*	m_pTechGuardian;

	double	m_dSceneChangeTime;
	double	m_dBgm = 0.0;
};


#endif