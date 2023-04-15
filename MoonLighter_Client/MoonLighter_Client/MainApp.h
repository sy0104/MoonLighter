#pragma once

#ifndef __MAINAPP_H__
#define __MAINAPP_H__


class CFrameMgr;

class CMainApp final
{
public:
	CMainApp();
	~CMainApp();

public:
	void Ready_MainApp();
	void Update_MainApp();
	void LateUpdate_MainApp();
	void Render_MainApp(const CFrameMgr& rFrameMgr);
	void Release_MainApp();
};


#endif