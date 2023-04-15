#pragma once

#ifndef __STAGE1_H__
#define __STAGE1_H__

#include "Scene.h"
class CStage1 : public CScene
{
public:
	explicit CStage1();
	virtual ~CStage1();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	CObj*	m_pPlayer;
	CObj*	m_pGolemKing;

	double	m_dSceneChangeTime;
};


#endif