#pragma once

#ifndef __SHOP_H__
#define __SHOP_H__

#include "Scene.h"
class CShop : public CScene
{
public:
	explicit CShop();
	virtual ~CShop();

public:
	virtual void Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;


private:
	CObj*	m_pPlayer;

	CObj*	m_pTable1;
	CObj*	m_pTable2;
	CObj*	m_pTable3;
	CObj*	m_pTable4;

	bool	m_bBgm;
};


#endif