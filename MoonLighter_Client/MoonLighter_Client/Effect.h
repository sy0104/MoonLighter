#pragma once
#include "Obj.h"

class CEffect : public CObj
{
public:
	explicit CEffect();
	virtual ~CEffect();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	void Set_EffectID(EFFECT::ID eID) { m_eCurID = eID; }


private:
	void Scene_Change();


private:
	EFFECT::ID		m_ePreID;
	EFFECT::ID		m_eCurID;

	wstring			m_wstrObjectKey;
	wstring			m_wstrStateKey;

	double			m_dFrameSpeed;
	bool			m_bCheck;

	bool			m_bSound;
	double			m_dSoundTime;
};

