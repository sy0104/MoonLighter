#include "stdafx.h"
#include "Shadow.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Punch.h"
#include "Rock.h"

#include "SoundMgr.h"

CShadow::CShadow()
	: m_wstrStateKey(L""), m_iKind(0), m_dDeathTime(0.0), m_dMoveTime(0.0), m_bCreatePunch(false), m_pPunch(nullptr)
	, m_bCreateRandomRock(false), m_bMove(false), m_dPauseTime(0.0), m_bRandomRock(false)
{
}


CShadow::~CShadow()
{
	Release_Object();
}

HRESULT CShadow::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_fSpeed = 6.f;

	CSoundMgr::Get_Instance()->PlaySound(L"Golem_Spawn.wav", CSoundMgr::EFFECT);

	return S_OK;
}

int CShadow::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_dDeathTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	// Rock Shadow
	if (m_wstrStateKey == L"ROCK_SHADOW")
	{
		if (m_bRandomRock && !m_bCreateRandomRock && m_dDeathTime > 2.0)
		{
			CObj* pObj = CAbstractFactory<CRock>::Create(m_tInfo.vPos.x, m_tInfo.vPos.y - 200.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ::ROCK, pObj);
			dynamic_cast<CRock*>(pObj)->Set_PosY(m_tInfo.vPos.y);

			m_bCreateRandomRock = true;
		}
		else if (m_dDeathTime > 7.0)
		{
			m_bDead = true;
			m_dDeathTime = 0.0;
		}
	}


	// Punch Shadow 크기조절
	if (m_wstrStateKey == L"PUNCH_SHADOW" && !m_bMove)
	{
		m_dMoveTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_tInfo.vSize.x < 2.0f && m_tInfo.vSize.y < 2.0f)
		{
			// 플레이어 따라옴
			CObj* pTarget = CObjMgr::Get_Instance()->Get_Player();

			m_tInfo.vDir = pTarget->Get_Info().vPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;

			// 커지기 - Punch 아래로 내려올때
			if (m_dMoveTime < 1.5)
			{
				m_tInfo.vSize.x += 0.003f;
				m_tInfo.vSize.y += 0.003f;
			}
			else
			{
				m_tInfo.vSize.x += 0.1f;
				m_tInfo.vSize.y += 0.1f;
			}
		}

		else
		{
			// Punch 생성
			if (!m_bCreatePunch)
			{
				m_pPunch = CAbstractFactory<CPunch>::Create(m_tInfo.vPos.x, -60.f);
				CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER_BACK, m_pPunch);
				dynamic_cast<CPunch*>(m_pPunch)->Set_PosY(m_tInfo.vPos.y - 50.f);

				m_bMove = true;
				m_bCreatePunch = true;
			}
		}
	}
	
	return OBJ_NOEVENT;
}

void CShadow::LateUpdate_Object()
{
	// 작아자기 - Punch 위로 올라갈때
	if (m_wstrStateKey == L"PUNCH_SHADOW" && m_bCreatePunch && m_bMove)
	{
		m_dPauseTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dPauseTime > 1.5 && m_tInfo.vSize.x > 0.f && m_tInfo.vSize.y > 0.f)
		{
			m_tInfo.vSize.x -= 0.1f;
			m_tInfo.vSize.y -= 0.1f;
		}

		else if (m_tInfo.vSize.x <= 0.f && m_tInfo.vSize.y <= 0.f)
			m_bDead = true;
	}
}

void CShadow::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	D3DXVECTOR3	vScroll = CScrollMgr::Get_Instance()->Get_Scroll();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"BOSS_A", m_wstrStateKey, 0);

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + vScroll.x, m_tInfo.vPos.y + vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CShadow::Release_Object()
{
}
