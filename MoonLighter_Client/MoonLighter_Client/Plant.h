#pragma once

#ifndef __PLANT_H__
#define __PLANT_H__

#include "Obj.h"
class CPlant : public CObj
{
public:
	explicit CPlant();
	virtual ~CPlant();

public:
	//		��Ʈ��, ���̵�, ��ƼŬ����, root����, ����, ȸ�����Ѿ�
	enum STATE { WAKEUP, IDLE, POTS, STEM, JUMP, SHAKE, DEATH, END };

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;

public:
	CObj* Get_PlantHead() { return m_pPlantHead; }
	CObj* Get_PlantBody() { return m_pPlantBody; }
	const bool& Get_Created() const { return m_bCreatePlants; }

private:
	void Scene_Change();
	void Initialize();

private:
	void Progress();			// ���� ���� ���� �Լ�

private:
	STATE		m_ePreScene;
	STATE		m_eCurScene;

	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;

	CObj*		m_pTarget;
	CObj*		m_pPlantExtra;
	CObj*		m_pPlantHead;
	CObj*		m_pPlantBody;

	double		m_dWaveTime;
	TCHAR		m_szWaveTime[32] = L"";

	bool		m_bCreatePlants;
	bool		m_bCreaetExtra;
	bool		m_bSetDead;

	// Pots
	bool		m_bPots;

	// Shake
	bool		m_bSetShake;

	// Jump
	bool		m_bJump;
	int			m_iJumpCnt;

};


#endif