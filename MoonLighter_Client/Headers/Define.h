#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX	1200
#define WINCY	800

#define TILECX	130
#define TILECY	68
#define TILEX	30
#define TILEY	20


#define OBJ_DEAD	1 
#define OBJ_NOEVENT 0

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

// 클라이언트에서 작업중이면, AFX는 MFC
#ifndef _AFX
#define ERR_MSG(Message) MessageBox(nullptr, Message, __T("ErrorMessage"), MB_OK)
#else
#define ERR_MSG(Message) AfxMessageBox(Message)
#endif


#define DECLARE_SINGLETON(Class)		\
public:									\
static Class* Get_Instance()			\
{										\
	if (nullptr == m_pInstance)			\
		m_pInstance = new Class;		\
	return m_pInstance;					\
}										\
static void Destroy_Instance()			\
{										\
	if (m_pInstance)					\
	{									\
		delete m_pInstance;				\
		m_pInstance = nullptr;			\
	}									\
}										\
private:								\
	static Class* m_pInstance;

#define IMPLEMENT_SINGLETON(Class)		\
Class* Class::m_pInstance = nullptr;



#endif