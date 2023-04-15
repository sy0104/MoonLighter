#pragma once

class CObj abstract
{
protected:
	explicit CObj();
public:
	virtual ~CObj();

public:
	virtual HRESULT Ready_Object() PURE;
	virtual int Update_Object() PURE;
	virtual void LateUpdate_Object() PURE;
	virtual void Render_Object() PURE;
	virtual void Release_Object() PURE;

protected:
	virtual void Frame_Move(const double& rFrameSpeed = 1.0);

public:
	const INFO& Get_Info() const { return m_tInfo; }
	const RECT& Get_Rect() const { return m_tRect; }
	const FRAME& Get_Frame() const { return m_tFrame; }
	const bool& Get_Dead() const { return m_bDead; }
	const UNITINFO& Get_UnitInfo() const { return m_tUnit; }
	const bool& Get_Collision() const { return m_bCollision; }
	const int& Get_Gold() const { return m_iGold; }
	const int& Get_Alpha() const { return m_iAlpha; }

public:
	void Set_Pos(float fX, float fY) { m_tInfo.vPos.x = fX; m_tInfo.vPos.y = fY; };
	void Set_Size(float fX, float fY) { m_tInfo.vSize.x = fX; m_tInfo.vSize.y = fY; }
	void Set_Angle(float fAngle) { m_fAngle = fAngle; }
	void Set_PosX(float fX) { m_tInfo.vPos.x += fX; }
	void Set_PosY(float fY) { m_tInfo.vPos.y += fY; }
	void Set_Dead() { m_bDead = true; }
	void Set_Hp(int _iAtt) { m_tUnit.m_iHp -= _iAtt; }
	void Set_Collision() { m_bCollision = false; }
	void Set_Alpha() { m_bAlpha = true; }
	void Set_AlphaValue(int iValue) { m_iAlpha = iValue; }
	void Set_Speed(float fSpeed) { m_fSpeed = fSpeed; }
	void Set_Gold(int iGold) { m_iGold += iGold; }


protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	UNITINFO	m_tUnit;

	float		m_fSpeed;
	float		m_fAngle;

	int			m_iAlpha;

	bool		m_bDead;
	bool		m_bCollision;
	bool		m_bAlpha;

	int			m_iGold;
};

