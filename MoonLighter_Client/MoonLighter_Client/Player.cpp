#include "stdafx.h"
#include "Player.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"
#include "KeyMgr.h"
#include "BigSword.h"
#include "Spear.h"
#include "Bow.h"
#include "Inven.h"

#include "SceneMgr.h"
#include "PlayerHp.h"
#include "PlayerHpBar.h"

CPlayer::CPlayer()
	: m_ePreScene(STATE_END), m_eCurScene(STATE_END), m_wstrObjectKey(L""), m_wstrStateKey(L""), m_bBreak(false), m_pPlayerHp(nullptr), m_pPlayerHpBar(nullptr)
	, m_ePlayerDir(DIR::END), m_bRoll(false), m_bAttack(false), m_bHit(false), m_dFrameSpeed(1.0), m_dCollisionBreak(0.0)
	, m_pWeapon(nullptr), m_bCreateWeapon(false), m_eWeaponState(WEAPON::BIGSWORD), m_bStop(false), m_pInven(nullptr), m_bCreateInven(false)
{
}


CPlayer::~CPlayer()
{
	Release_Object();
}

HRESULT CPlayer::Ready_Object()
{
	m_tInfo.vPos = { 900.f, 600.f, 0.f };
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f }; 
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 7.f;

	m_ePreScene = IDLE_UP;
	m_eCurScene = IDLE_UP;
	m_wstrObjectKey = L"Player_Idle";
	m_wstrStateKey = L"Up";
	m_tFrame = { 0.0, 10.0 };

	m_tUnit = { 5, 100, 100 };
	m_iAlpha = 255;

	//m_pInven = CAbstractFactory<CInven>::Create();
	//CObjMgr::Get_Instance()->Add_Object(OBJ::INVEN, m_pInven);

	// Inven
	if (CObjMgr::Get_Instance()->Get_List(OBJ::INVEN).empty())
	{
		m_pInven = CAbstractFactory<CInven>::Create();
		CObjMgr::Get_Instance()->Add_Object(OBJ::INVEN, m_pInven);
	}
	else
	{
		m_pInven = CObjMgr::Get_Instance()->Get_List(OBJ::INVEN).front();
	}


	// UI //
	// PlayerHpBar
	m_pPlayerHpBar = CAbstractFactory<CPlayerHpBar>::Create(200.f, 50.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pPlayerHpBar);

	// PlayerHp
	m_pPlayerHp = CAbstractFactory<CPlayerHp>::Create();
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, m_pPlayerHp);


	return S_OK;
}

int CPlayer::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;
	
	if (m_bAttack && m_eWeaponState == WEAPON::BOW)
		m_dFrameSpeed = 1.5;
	else
		m_dFrameSpeed = 1.0;

	if (m_bStop)
		m_fSpeed = 0.f;
	else
		m_fSpeed = 7.f;

	if (m_bRoll)
		m_bCollision = false;

	// Hp Bar
	if (m_tUnit.m_iHp >= 0)
		dynamic_cast<CPlayerHp*>(m_pPlayerHp)->Set_HpRect((LONG)(m_tUnit.m_iHp));


	KeyCheck();
	if (CSceneMgr::Get_Instance()->Get_Scroll())
		OffSet();
	Frame_Move(m_dFrameSpeed);
	Scene_Change();

	return OBJ_NOEVENT;
}

void CPlayer::LateUpdate_Object()
{
	// 충돌
	if (!m_bBreak)
	{
		if (!m_bCollision)
		{
			m_iAlpha = 150;

			m_dCollisionBreak += CTimeMgr::Get_Instance()->Get_DeltaTime();

			if (m_dCollisionBreak >= 1.0)
			{
				m_bCollision = true;
				m_dCollisionBreak = 0.0;
			}
		}

		else if (m_bCollision)
			m_iAlpha = 255;
	}


	// 충돌 모션
	if (m_bHit)
	{
		if (m_ePlayerDir == DIR::LEFT)
		{
			m_eCurScene = HIT_LEFT;
			m_tInfo.vPos.x += m_tInfo.vDir.x * (m_fSpeed * 0.3f);
		}
		else if (m_ePlayerDir == DIR::RIGHT)
		{
			m_eCurScene = HIT_RIGHT;
			m_tInfo.vPos.x -= m_tInfo.vDir.x * (m_fSpeed * 0.3f);
		}
		else if (m_ePlayerDir == DIR::UP)
		{
			m_eCurScene = HIT_UP;
			m_tInfo.vPos.y += m_tInfo.vDir.y * (m_fSpeed * 0.3f);
		}
		else if (m_ePlayerDir == DIR::DOWN)
		{
			m_eCurScene = HIT_DOWN;
			m_tInfo.vPos.y -= m_tInfo.vDir.y * (m_fSpeed * 0.3f);
		}
	}
}


void CPlayer::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 2.f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 2.f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 2.f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 2.f));


//#ifdef _DEBUG
//
//	// 플레이어 좌표 출력
//	D3DXMATRIX	matFontTrans;
//	D3DXMatrixTranslation(&matFontTrans, 100.f, 100.f, 0.f);
//	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matFontTrans);
//
//	swprintf_s(m_szPos, L"(p)X: %f   Y: %f", m_tInfo.vPos.x, m_tInfo.vPos.y);
//	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szPos, lstrlen(m_szPos), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
//
//	// 충돌박스 출력
//
//	D3DXVECTOR2 vLine[5] =
//	{
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.top + vScroll.y },
//		{ (float)m_tRect.right + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.bottom + vScroll.y },
//		{ (float)m_tRect.left + vScroll.x, (float)m_tRect.top + vScroll.y }
//	};
//
//	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
//	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
//	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
//
//#endif



	// 플레이어 HP 출력
	D3DXMATRIX	matHpTrans;
	D3DXMatrixTranslation(&matHpTrans, 210.f, 70.f, 0.f);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matHpTrans);

	swprintf_s(m_szHp, L"%d / %d", m_tUnit.m_iHp, m_tUnit.m_iMaxHp);
	CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_szHp, lstrlen(m_szHp), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayer::Release_Object()
{
}

void CPlayer::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_bRoll)
		{
			m_bRoll = false;
			m_dFrameSpeed = 1.0;
		}

		if (m_bAttack)
		{
			m_bAttack = false;
			m_bCreateWeapon = false;
		}

		if (m_bHit)
		{
			if (m_ePlayerDir == DIR::LEFT)
				m_eCurScene = IDLE_LEFT;
			else if (m_ePlayerDir == DIR::RIGHT)
				m_eCurScene = IDLE_RIGHT;
			else if (m_ePlayerDir == DIR::UP)
				m_eCurScene = IDLE_UP;
			else if (m_ePlayerDir == DIR::DOWN)
				m_eCurScene = IDLE_DOWN;

			m_bHit = false;
		}

		m_tFrame.dFrameStart = 0.0;
	}

}

CObj* CPlayer::Get_Weapon()
{
	if (m_pWeapon)
		return m_pWeapon;
	else
		return nullptr;
}


void CPlayer::KeyCheck()
{
	// -----------------------------------------------------------------------------------
	//// Walk
	// Left
	if (!m_bAttack && !m_bHit && CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_ePlayerDir = DIR::LEFT;
		m_tInfo.vPos.x -= m_tInfo.vDir.x * m_fSpeed;

		// Roll_Left
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bRoll = true;
			m_eCurScene = ROLL_LEFT;
			m_dFrameSpeed = 1.5;
		}
		// Walk_Left
		else if (!m_bRoll)
			m_eCurScene = WALK_LEFT;
	}

	// Right
	else if (!m_bAttack && !m_bHit && CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_ePlayerDir = DIR::RIGHT;
		m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed;

		// Roll_Right
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bRoll = true;
			m_eCurScene = ROLL_RIGHT;
			m_dFrameSpeed = 1.5;
		}
		// Walk_Right
		else if (!m_bRoll)
			m_eCurScene = WALK_RIGHT;
	}

	// Up
	else if (!m_bAttack && !m_bHit && CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_ePlayerDir = DIR::UP;
		m_tInfo.vPos.y -= m_tInfo.vDir.y * m_fSpeed;

		// Roll_Up
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bRoll = true;
			m_eCurScene = ROLL_UP;
			m_dFrameSpeed = 1.5;
		}
		// Walk_Up
		else if (!m_bRoll)
			m_eCurScene = WALK_UP;
	}

	// Down
	else if (!m_bAttack && !m_bHit && CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_ePlayerDir = DIR::DOWN;
		m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed;

		// Roll_Down
		if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
		{
			m_bRoll = true;
			m_eCurScene = ROLL_DOWN;
			m_dFrameSpeed = 1.5;
		}
		// Walk_Down
		else if (!m_bRoll)
			m_eCurScene = WALK_DOWN;
	}
	// -----------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------
	// Attack
	else if (!m_bHit && CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		m_bAttack = true;

		if (m_ePlayerDir == DIR::LEFT)
		{
			m_eCurScene = ATTACK_LEFT;

			if (!m_bCreateWeapon)
			{
				switch (m_eWeaponState)
				{
					case WEAPON::BIGSWORD:
					{
						m_pWeapon = CAbstractFactory<CBigSword>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
						CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
						dynamic_cast<CBigSword*>(m_pWeapon)->Set_StateKey(L"LEFT");
					}
					break;
					case WEAPON::SPEAR:
					{
						m_pWeapon = CAbstractFactory<CSpear>::Create(m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y);
						CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
						dynamic_cast<CSpear*>(m_pWeapon)->Set_StateKey(L"LEFT");
					}
					break;
					case WEAPON::BOW:
					{
						m_pWeapon = CAbstractFactory<CBow>::Create(m_tInfo.vPos.x - 30.f, m_tInfo.vPos.y);
						CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
						dynamic_cast<CBow*>(m_pWeapon)->Set_StateKey(L"LEFT");
					}
					break;
				}

				m_bCreateWeapon = true;
			}
		}
		else if (m_ePlayerDir == DIR::RIGHT)
		{
			m_eCurScene = ATTACK_RIGHT;

			if (!m_bCreateWeapon)
			{
				switch (m_eWeaponState)
				{
				case WEAPON::BIGSWORD:
				{
					m_pWeapon = CAbstractFactory<CBigSword>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBigSword*>(m_pWeapon)->Set_StateKey(L"RIGHT");
				}
				break;
				case WEAPON::SPEAR:
				{
					m_pWeapon = CAbstractFactory<CSpear>::Create(m_tInfo.vPos.x + 50.f, m_tInfo.vPos.y);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CSpear*>(m_pWeapon)->Set_StateKey(L"RIGHT");
				}
				break;
				case WEAPON::BOW:
				{
					m_pWeapon = CAbstractFactory<CBow>::Create(m_tInfo.vPos.x + 30.f, m_tInfo.vPos.y);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBow*>(m_pWeapon)->Set_StateKey(L"RIGHT");
				}
				break;
				}

				m_bCreateWeapon = true;
			}
		}
		else if (m_ePlayerDir == DIR::UP)
		{
			m_eCurScene = ATTACK_UP;
		
			if (!m_bCreateWeapon)
			{
				switch (m_eWeaponState)
				{
				case WEAPON::BIGSWORD:
				{
					m_pWeapon = CAbstractFactory<CBigSword>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBigSword*>(m_pWeapon)->Set_StateKey(L"UP");
				}
				break;
				case WEAPON::SPEAR:
				{
					m_pWeapon = CAbstractFactory<CSpear>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 50.f);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CSpear*>(m_pWeapon)->Set_StateKey(L"UP");
				}
				break;
				case WEAPON::BOW:
				{
					m_pWeapon = CAbstractFactory<CBow>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 30.f);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBow*>(m_pWeapon)->Set_StateKey(L"UP");
				}
				break;
				}

				m_bCreateWeapon = true;
			}
		}
		else
		{
			m_eCurScene = ATTACK_DOWN;

			if (!m_bCreateWeapon)
			{
				switch (m_eWeaponState)
				{
				case WEAPON::BIGSWORD:
				{
					m_pWeapon = CAbstractFactory<CBigSword>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBigSword*>(m_pWeapon)->Set_StateKey(L"DOWN");
				}
				break;
				case WEAPON::SPEAR:
				{
					m_pWeapon = CAbstractFactory<CSpear>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 50.f);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CSpear*>(m_pWeapon)->Set_StateKey(L"DOWN");
				}
				break;
				case WEAPON::BOW:
				{
					m_pWeapon = CAbstractFactory<CBow>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y + 30.f);
					CObjMgr::Get_Instance()->Add_Object(OBJ::WEAPON, m_pWeapon);
					dynamic_cast<CBow*>(m_pWeapon)->Set_StateKey(L"DOWN");
				}
				break;
				}

				m_bCreateWeapon = true;
			}
		}
	}
	// -----------------------------------------------------------------------------------
	// 무기 변경
	else if (!m_bHit && CKeyMgr::Get_Instance()->Key_Down(VK_TAB))
	{
		// 검 -> 창
		if (m_eWeaponState == WEAPON::BIGSWORD)
			m_eWeaponState = WEAPON::SPEAR;

		// 창 -> 활
		else if (m_eWeaponState == WEAPON::SPEAR)
			m_eWeaponState = WEAPON::BOW;

		// 활 -> 검
		else if (m_eWeaponState == WEAPON::BOW)
			m_eWeaponState = WEAPON::BIGSWORD;
	}
	// -----------------------------------------------------------------------------------
	// Idle
	else
	{
		if (!m_bRoll && !m_bAttack && !m_bHit)
		{
			if (m_ePlayerDir == DIR::LEFT)
				m_eCurScene = IDLE_LEFT;
			else if (m_ePlayerDir == DIR::RIGHT)
				m_eCurScene = IDLE_RIGHT;
			else if (m_ePlayerDir == DIR::UP)
				m_eCurScene = IDLE_UP;
			else
				m_eCurScene = IDLE_DOWN;
		}

	}
	// -----------------------------------------------------------------------------------
	// Stop
	if (m_bStop)
	{
		if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)
			|| CKeyMgr::Get_Instance()->Key_Up(VK_UP) || CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
			m_bStop = false;

		//if (m_ePlayerDir == DIR::LEFT)
		//{
		//	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		//		m_bStop = false;
		//}
	}
	// -----------------------------------------------------------------------------------
	// Inven
	if (CKeyMgr::Get_Instance()->Key_Down('I'))
	{
		if (!m_bCreateInven)
		{
			dynamic_cast<CInven*>(m_pInven)->Set_Render();

			m_bCreateInven = true;
		}

		else if (m_bCreateInven)
		{
			dynamic_cast<CInven*>(m_pInven)->Set_Hide();

			m_bCreateInven = false;
		}
	}
	// -----------------------------------------------------------------------------------
	// 무적
	if (CKeyMgr::Get_Instance()->Key_Down('B'))
	{
		if (m_bBreak)
			m_bBreak = false;
		else if (!m_bBreak)
			m_bBreak = true;
	}
	// -----------------------------------------------------------------------------------
}

void CPlayer::OffSet()
{
	int iOffSetX = WINCX >> 1;
	int iOffSetY = WINCY >> 1;

	D3DXVECTOR3 vScroll = CScrollMgr::Get_Instance()->Get_Scroll();

	if (iOffSetX < m_tInfo.vPos.x + vScroll.x)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.vPos.x + vScroll.x));
	if (iOffSetX > m_tInfo.vPos.x + vScroll.x)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffSetX - (m_tInfo.vPos.x + vScroll.x));

	if (iOffSetY < m_tInfo.vPos.y + vScroll.y)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffSetY - (m_tInfo.vPos.y + vScroll.y));
	if (iOffSetY > m_tInfo.vPos.y + vScroll.y)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffSetY - (m_tInfo.vPos.y + vScroll.y));
}

void CPlayer::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		//// IDLE ////
		case CPlayer::IDLE_LEFT:
			m_wstrObjectKey = L"Player_Idle";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 10.0 };
			break;
		case CPlayer::IDLE_RIGHT:
			m_wstrObjectKey = L"Player_Idle";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 10.0 };
			break;
		case CPlayer::IDLE_UP:
			m_wstrObjectKey = L"Player_Idle";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 10.0 };
			break;
		case CPlayer::IDLE_DOWN:
			m_wstrObjectKey = L"Player_Idle";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 10.0 };
			break;

		//// WALK ////
		case CPlayer::WALK_LEFT:
			m_wstrObjectKey = L"Player_Walk";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::WALK_RIGHT:
			m_wstrObjectKey = L"Player_Walk";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::WALK_UP:
			m_wstrObjectKey = L"Player_Walk";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::WALK_DOWN:
			m_wstrObjectKey = L"Player_Walk";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 8.0 };
			break;

		//// ROLL ////
		case CPlayer::ROLL_LEFT:
			m_wstrObjectKey = L"Player_Roll";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::ROLL_RIGHT:
			m_wstrObjectKey = L"Player_Roll";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::ROLL_UP:
			m_wstrObjectKey = L"Player_Roll";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 8.0 };
			break;
		case CPlayer::ROLL_DOWN:
			m_wstrObjectKey = L"Player_Roll";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 8.0 };
			break;

		//// ATTACK ////
		case CPlayer::ATTACK_LEFT:
			if (m_eWeaponState == WEAPON::BIGSWORD)
			{
				m_wstrObjectKey = L"Attack_BigSword";
				m_tFrame = { 0.0, 40.0 };
			}
			else if (m_eWeaponState == WEAPON::SPEAR)
			{
				m_wstrObjectKey = L"Attack_Spear";
				m_tFrame = { 0.0, 23.0 };
			}
			else
			{
				m_wstrObjectKey = L"Attack_Bow";
				m_tFrame = { 0.0, 11.0 };
			}
			m_wstrStateKey = L"Left";
			break;
		case CPlayer::ATTACK_RIGHT:
			if (m_eWeaponState == WEAPON::BIGSWORD)
			{
				m_wstrObjectKey = L"Attack_BigSword";
				m_tFrame = { 0.0, 40.0 };
			}
			else if (m_eWeaponState == WEAPON::SPEAR)
			{ 
				m_wstrObjectKey = L"Attack_Spear";
				m_tFrame = { 0.0, 23.0 };
			}
			else
			{
				m_wstrObjectKey = L"Attack_Bow";
				m_tFrame = { 0.0, 11.0 };
			}
			m_wstrStateKey = L"Right";
			break;
		case CPlayer::ATTACK_UP:
			if (m_eWeaponState == WEAPON::BIGSWORD)
			{
				m_wstrObjectKey = L"Attack_BigSword";
				m_tFrame = { 0.0, 40.0 };
			}
			else if (m_eWeaponState == WEAPON::SPEAR)
			{
				m_wstrObjectKey = L"Attack_Spear";
				m_tFrame = { 0.0, 23.0 };
			}
			else
			{
				m_wstrObjectKey = L"Attack_Bow";
				m_tFrame = { 0.0, 11.0 };
			}
			m_wstrStateKey = L"Up";
			break;
		case CPlayer::ATTACK_DOWN:
			if (m_eWeaponState == WEAPON::BIGSWORD)
			{
				m_wstrObjectKey = L"Attack_BigSword";
				m_tFrame = { 0.0, 40.0 };
			}
			else if (m_eWeaponState == WEAPON::SPEAR)
			{
				m_wstrObjectKey = L"Attack_Bow";
				m_tFrame = { 0.0, 11.0 };
			}
			else
			{
				m_wstrObjectKey = L"Attack_Bow";
				m_tFrame = { 0.0, 11.0 };
			}
			m_wstrStateKey = L"Down";
			break;
		//// Hit ////
		case CPlayer::HIT_LEFT:
			m_wstrObjectKey = L"Player_Damaged";
			m_wstrStateKey = L"Left";
			m_tFrame = { 0.0, 9.0 };
			break;
		case CPlayer::HIT_RIGHT:
			m_wstrObjectKey = L"Player_Damaged";
			m_wstrStateKey = L"Right";
			m_tFrame = { 0.0, 9.0 };
			break;
		case CPlayer::HIT_UP:
			m_wstrObjectKey = L"Player_Damaged";
			m_wstrStateKey = L"Up";
			m_tFrame = { 0.0, 9.0 };
			break;
		case CPlayer::HIT_DOWN:
			m_wstrObjectKey = L"Player_Damaged";
			m_wstrStateKey = L"Down";
			m_tFrame = { 0.0, 9.0 };
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}