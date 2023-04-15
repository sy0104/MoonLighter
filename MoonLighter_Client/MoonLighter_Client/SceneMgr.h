#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

class CScene;

class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr();
	~CSceneMgr();

public:
	enum SCENEID { LOGO, TOWN, SHOP, MAP, PRESTAGE, STAGE1, STAGE2, STAGE3, END };

public:
	void Update_SceneMgr();
	void LateUpdate_SceneMgr();
	void Render_SceneMgr();
	void Release_SceneMgr();

public:
	bool Get_Scroll();

public:
	HRESULT Change_Scene(SCENEID eID);

private:
	CScene*		m_pScene;
	SCENEID		m_ePreScene;
	SCENEID		m_eCurScene;
};



#endif