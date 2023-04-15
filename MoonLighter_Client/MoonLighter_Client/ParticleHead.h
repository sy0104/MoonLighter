#pragma once

#ifndef __PARTICLEHEAD_H__
#define __PARTICLEHEAD_H__

#include "Obj.h"
class CParticleHead : public CObj
{
public:
	explicit CParticleHead();
	virtual ~CParticleHead();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_State(PARTICLE::HEAD eState) { m_eCurScene = eState; }

private:
	void Scene_Change();
	void Progress();

private:
	PARTICLE::HEAD		m_ePreScene;
	PARTICLE::HEAD		m_eCurScene;

	wstring				m_wstrStateKey;

	D3DXMATRIX			matScale, matRotZ, matTrans, matWorld;

	double				m_dBulletTime;
	double				m_dHitTime;

	bool				m_bCreateBullet;
	CObj*				m_pParticle_Body;

	TCHAR				m_szHp[32] = L"";
};


#endif