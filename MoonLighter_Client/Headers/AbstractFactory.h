#pragma once

#ifndef __ABSTRACTFACTORY_H__
#define __ABSTRACTFACTORY_H__

class CObj;

template <typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Ready_Object();

		return pObj;
	}

	static CObj* Create(float _x, float _y)
	{
		CObj* pObj = new T;
		pObj->Ready_Object();
		pObj->Set_Pos(_x, _y);

		return pObj;
	}

	static CObj* Create(float _x, float _y, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->Ready_Object();
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_fAngle);

		return pObj;
	}

};

#endif