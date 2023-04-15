#pragma once

class CInven;

class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();
	~CObjMgr();

public:
	void Add_Object(OBJ::ID eID, CObj* pObj);

public:
	void Update_ObjMgr();
	void LateUpdate_ObjMgr();
	void Render_ObjMgr();
	void Release_ObjMgr();

public:
	CObj* Get_Player();
	CObj* Get_Object(OBJ::ID eID) { return m_listObj[eID].front(); }
	list<CObj*>& Get_List(OBJ::ID eID) { return m_listObj[eID]; }
	size_t Get_ObjSize(OBJ::ID eID) { return m_listObj[eID].size(); }

public:
	void Delete_ID(OBJ::ID eID);

private:
	list<CObj*>		m_listObj[OBJ::END];

};

