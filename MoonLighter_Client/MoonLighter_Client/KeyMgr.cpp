#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;
CKeyMgr::CKeyMgr()
{
	ZeroMemory(bKeyState, sizeof(bKeyState));
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
}

bool CKeyMgr::Key_Down(int _key)
{
	if (!bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		bKeyState[_key] = !bKeyState[_key];
		return true;
	}
	return false;
}

bool CKeyMgr::Key_Up(int _key)
{
	if (bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		bKeyState[_key] = !bKeyState[_key];
		return true;
	}
	return false;
}

void CKeyMgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			bKeyState[i] = !bKeyState[i];
		if (!bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			bKeyState[i] = !bKeyState[i];
	}
}
