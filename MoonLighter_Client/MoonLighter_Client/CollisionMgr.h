#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;

class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void Collision_Rect(CObj* _Dst, list<CObj*>& _Src);

	// °øÅë
	static void Collision_Player_To_Boss(CObj* _pPlayer, CObj* _pBoss);
	static void Collision_Player_To_Monster(CObj* _pPlayer, list<CObj*>& _pMonster);
	static void Collision_Player_To_Item(CObj* _pPlayer, list<CObj*>& _pItem);

	static void Collision_Weapon_To_Boss(CObj* _pPlayer, CObj* _pBoss);
	static void Collision_Weapon_To_Monster(CObj* _pPlayer, list<CObj*>& _pMonster);

	static void Collision_Arrow_To_Boss(list<CObj*>& _pArrow, CObj* _pBoss);
	static void Collision_Arrow_To_Monster(list<CObj*>& _pArrow, list<CObj*>& _pMonster);


	// Stage1: Player & Rock
	static void Collision_Player_To_Rock(list<CObj*>& _pRock, CObj* _pPlayer);

	// Shop: Player & Table
	static void Collisoin_Player_To_Table(CObj* _pPlayer, list<CObj*>& _pTable);

};


#endif