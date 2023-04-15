#pragma once

#ifndef __MAINMAP_H__
#define __MAINMAP_H__

#include "Scene.h"
class CMainMap : public CScene
{
public:
	explicit CMainMap();
	virtual ~CMainMap();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

private:
	CObj*	m_pPlayer;
};


#endif