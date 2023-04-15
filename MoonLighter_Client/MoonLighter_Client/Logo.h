#pragma once

#ifndef __LOGO_H__
#define __LOGO_H__

#include "Scene.h"
class CLogo : public CScene
{
public:
	explicit CLogo();
	virtual ~CLogo();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	void Frame_Move();

private:
	FRAME	m_tFrame;
};


#endif
