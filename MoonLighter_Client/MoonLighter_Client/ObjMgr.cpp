#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"


IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release_ObjMgr();
}

void CObjMgr::Add_Object(OBJ::ID eID, CObj * pObj)
{
	m_listObj[eID].emplace_back(pObj);
}

void CObjMgr::Update_ObjMgr()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		auto& iter = m_listObj[i].begin();
		auto& iter_end = m_listObj[i].end();

		for (; iter != iter_end; )
		{
			int iEvent = (*iter)->Update_Object();
			if (iEvent == OBJ_DEAD)
			{
				Safe_Delete(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}

	}
}

void CObjMgr::LateUpdate_ObjMgr()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->LateUpdate_Object();

			if (m_listObj[i].empty())
				break;

		}
	}
}

void CObjMgr::Render_ObjMgr()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
			pObj->Render_Object();
	}


}

void CObjMgr::Release_ObjMgr()
{
	for (int i = 0; i < OBJ::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			Safe_Delete(pObj);
		}
		m_listObj[i].clear();
	}
}

CObj* CObjMgr::Get_Player()
{
	if (m_listObj[OBJ::TOWN_PLAYER].empty())
		return m_listObj[OBJ::PLAYER].front();
	else
		return m_listObj[OBJ::TOWN_PLAYER].front();
}

void CObjMgr::Delete_ID(OBJ::ID eID)
{
	for_each(m_listObj[eID].begin(), m_listObj[eID].end(), Safe_Delete<CObj*>);
	m_listObj[eID].clear();
}
