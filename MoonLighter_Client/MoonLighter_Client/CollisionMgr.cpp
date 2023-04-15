#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Potal.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "Player.h"
#include "TownPlayer.h"
#include "Rock.h"
#include "GolemKing.h"
#include "ObjMgr.h"
#include "Inven.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(CObj* _Dst, list<CObj*>& _Src)
{
	RECT rc = {};


	for (auto& Src : _Src)
	{
		if (IntersectRect(&rc, &(_Dst->Get_Rect()), &(Src->Get_Rect())))
		{
			Src->Set_Hp(_Dst->Get_UnitInfo().m_iAtt);
			_Dst->Set_Dead();
			//Src->Set_Dead();
		}

	}
}

void CCollisionMgr::Collision_Player_To_Boss(CObj* _pPlayer, CObj* _pBoss)
{
	RECT rc = {};

	if (IntersectRect(&rc, &(_pPlayer->Get_Rect()), &(_pBoss->Get_Rect())))
	{
		if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Collision())
		{
			_pPlayer->Set_Hp(_pBoss->Get_UnitInfo().m_iAtt);
			dynamic_cast<CPlayer*>(_pPlayer)->Set_Hit();
			_pPlayer->Set_Collision();
		}
	}
}

void CCollisionMgr::Collision_Player_To_Monster(CObj* _pPlayer, list<CObj*>& _pMonster)
{
	RECT rc = {};

	for (auto& pMonster : _pMonster)
	{
		if (IntersectRect(&rc, &(_pPlayer->Get_Rect()), &(pMonster->Get_Rect())))
		{
			if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Collision())
			{
				_pPlayer->Set_Hp(pMonster->Get_UnitInfo().m_iAtt);
				dynamic_cast<CPlayer*>(_pPlayer)->Set_Hit();
				_pPlayer->Set_Collision();
			}
		}
	}
}

void CCollisionMgr::Collision_Player_To_Item(CObj* _pPlayer, list<CObj*>& _pItem)
{
	RECT rc = {};

	for (auto& pItem : _pItem)
	{
		if (IntersectRect(&rc, &(_pPlayer->Get_Rect()), &(pItem->Get_Rect())))
		{
			CObj* pInven = CObjMgr::Get_Instance()->Get_Object(OBJ::INVEN);
			dynamic_cast<CInven*>(pInven)->Insert_Item(pItem);

			pItem->Set_Dead();
		}
	}
}

void CCollisionMgr::Collision_Weapon_To_Boss(CObj* _pPlayer, CObj* _pBoss)
{
	RECT rc = {};
	
	CObj* pWeapon = dynamic_cast<CPlayer*>(_pPlayer)->Get_Weapon();

	if (pWeapon)
	{
		if (IntersectRect(&rc, &(pWeapon->Get_Rect()), &(_pBoss->Get_Rect())))
		{
			// Big Sword
			if (dynamic_cast<CPlayer*>(_pPlayer)->Get_WeaponID() == WEAPON::BIGSWORD)
			{
				if (pWeapon->Get_Frame().dFrameStart >= 5.0 && pWeapon->Get_Frame().dFrameStart <= 5.5 ||
					pWeapon->Get_Frame().dFrameStart >= 16.0 && pWeapon->Get_Frame().dFrameStart <= 16.5 ||
					pWeapon->Get_Frame().dFrameStart >= 27.0 && pWeapon->Get_Frame().dFrameStart <= 27.5)
				{
					_pBoss->Set_Alpha();
					_pBoss->Set_Hp(pWeapon->Get_UnitInfo().m_iAtt);
				}
			}

			// Spear
			else if (dynamic_cast<CPlayer*>(_pPlayer)->Get_WeaponID() == WEAPON::SPEAR)
			{
				if (pWeapon->Get_Frame().dFrameStart >= 0.0 && pWeapon->Get_Frame().dFrameStart <= 0.5 ||
					pWeapon->Get_Frame().dFrameStart >= 7.0 && pWeapon->Get_Frame().dFrameStart <= 7.5 ||
					pWeapon->Get_Frame().dFrameStart >= 15.0 && pWeapon->Get_Frame().dFrameStart <= 15.5)
				{
					_pBoss->Set_Alpha();
					_pBoss->Set_Hp(pWeapon->Get_UnitInfo().m_iAtt);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Weapon_To_Monster(CObj* _pPlayer, list<CObj*>& _pMonster)
{
	RECT rc = {};

	CObj* pWeapon = dynamic_cast<CPlayer*>(_pPlayer)->Get_Weapon();

	if (pWeapon)
	{
		for (auto& pMonster : _pMonster)
		{
			if (IntersectRect(&rc, &(pWeapon->Get_Rect()), &(pMonster->Get_Rect())))
			{
				// Big Sword
				if (dynamic_cast<CPlayer*>(_pPlayer)->Get_WeaponID() == WEAPON::BIGSWORD)
				{
					if (pWeapon->Get_Frame().dFrameStart >= 5.0 && pWeapon->Get_Frame().dFrameStart <= 5.5 ||
						pWeapon->Get_Frame().dFrameStart >= 16.0 && pWeapon->Get_Frame().dFrameStart <= 16.5 ||
						pWeapon->Get_Frame().dFrameStart >= 27.0 && pWeapon->Get_Frame().dFrameStart <= 27.5)
					{
						pMonster->Set_Alpha();
						pMonster->Set_Hp(pWeapon->Get_UnitInfo().m_iAtt);
					}
				}

				// Spear
				else if (dynamic_cast<CPlayer*>(_pPlayer)->Get_WeaponID() == WEAPON::SPEAR)
				{
					if (pWeapon->Get_Frame().dFrameStart >= 0.0 && pWeapon->Get_Frame().dFrameStart <= 0.5 ||
						pWeapon->Get_Frame().dFrameStart >= 7.0 && pWeapon->Get_Frame().dFrameStart <= 7.5 ||
						pWeapon->Get_Frame().dFrameStart >= 15.0 && pWeapon->Get_Frame().dFrameStart <= 15.5)
					{
						pMonster->Set_Alpha();
						pMonster->Set_Hp(pWeapon->Get_UnitInfo().m_iAtt);
					}
				}

			}
		}

	}

}

void CCollisionMgr::Collision_Arrow_To_Boss(list<CObj*>& _pArrow, CObj* _pBoss)
{
	RECT rc = {};

	for (auto& pArrow : _pArrow)
	{
		if (IntersectRect(&rc, &(pArrow->Get_Rect()), &(_pBoss->Get_Rect())))
		{
			_pBoss->Set_Alpha();
			_pBoss->Set_Hp(pArrow->Get_UnitInfo().m_iAtt);
			pArrow->Set_Dead();
		}
	}
}

void CCollisionMgr::Collision_Arrow_To_Monster(list<CObj*>& _pArrow, list<CObj*>& _pMonster)
{
	RECT rc = {};

	for (auto& pArrow : _pArrow)
	{
		for (auto& pMonster : _pMonster)
		{
			if (IntersectRect(&rc, &(pArrow->Get_Rect()), &(pMonster->Get_Rect())))
			{
				pMonster->Set_Alpha();
				pMonster->Set_Hp(pArrow->Get_UnitInfo().m_iAtt);
				pArrow->Set_Dead();
			}
		}
	}
}



void CCollisionMgr::Collision_Player_To_Rock(list<CObj*>& _pRock, CObj* _pPlayer)
{
	RECT rc = {};

	for (auto& pRock : _pRock)
	{
		if (&(pRock->Get_Collision()))
		{
			if (IntersectRect(&rc, &(pRock->Get_Rect()), &(_pPlayer->Get_Rect())))
			{
				// 왼쪽
				if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Dir() == DIR::LEFT)
				{
					if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Stop();
					else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Move();


				}
				// 오른쪽
				else if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Dir() == DIR::RIGHT)
				{
					if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Stop();
					else //if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Move();
				}
				// 위
				else if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Dir() == DIR::UP)
				{
					if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Stop();
					else //if (CKeyMgr::Get_Instance()->Key_Up(VK_UP))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Move();
				}

				// 아래
				else //if (dynamic_cast<CPlayer*>(_pPlayer)->Get_Dir() == DIR::DOWN)
				{
					if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Stop();
					else// if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
						dynamic_cast<CPlayer*>(_pPlayer)->Set_Move();
				}
			}
		}
	}
}

void CCollisionMgr::Collisoin_Player_To_Table(CObj* _pPlayer, list<CObj*>& _pTable)
{
	RECT rc = {};

	for (auto& pTable : _pTable)
	{
		if (IntersectRect(&rc, &(_pPlayer->Get_Rect()), &(pTable->Get_Rect())))
		{
			// 왼쪽
			if (dynamic_cast<CTownPlayer*>(_pPlayer)->Get_Dir() == DIR::LEFT)
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Stop();
				else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Move();


			}
			// 오른쪽
			else if (dynamic_cast<CTownPlayer*>(_pPlayer)->Get_Dir() == DIR::RIGHT)
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Stop();
				else
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Move();
			}
			// 위
			else if (dynamic_cast<CTownPlayer*>(_pPlayer)->Get_Dir() == DIR::UP)
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Stop();
				else
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Move();
			}

			// 아래
			else
			{
				if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Stop();
				else
					dynamic_cast<CTownPlayer*>(_pPlayer)->Set_Move();
			}
		}
	}

}
