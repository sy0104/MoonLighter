#include "stdafx.h"
#include "Slime.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"


CSlime::CSlime()
	: m_eCurScene(END), m_ePreScene(END), m_wstrStateKey(L""), m_dHitTime(0.0), m_bInitFrame(false)
{
	ZeroMemory(&m_tRange, sizeof(m_tRange));
}


CSlime::~CSlime()
{
	Release_Object();
}

HRESULT CSlime::Ready_Object()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 0.5f, 0.5f, 0.f };
	m_tInfo.vDir = { 1.f, 1.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };

	m_ePreScene = IDLE;
	m_eCurScene = IDLE;
	m_wstrStateKey = L"IDLE";
	m_tFrame = { 0.0, 9.0 };

	m_tUnit = { 5, 20, 20 };

	m_fSpeed = 2.f;

	return S_OK;
}

int CSlime::Update_Object()
{
	if (m_bDead)
		return OBJ_DEAD;

	// 범위
	m_tRange = { (LONG)(m_tInfo.vPos.x - 100.f),
				 (LONG)(m_tInfo.vPos.y - 100.f),
				 (LONG)(m_tInfo.vPos.x + 100.f),
				 (LONG)(m_tInfo.vPos.y + 100.f) };


	// 피격
	if (m_bAlpha && m_tUnit.m_iHp > 0 && m_eCurScene != DEATH)
	{
		m_dHitTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

		if (m_dHitTime >= 0.5)
		{
			m_bAlpha = false;
			m_dHitTime = 0.0;
		}
	}

	// 죽음
	if (m_tUnit.m_iHp <= 0)
		m_eCurScene = DEATH;


	// 플레이어가 범위 안이면 따라감
	if (IntersectRect(&rc, &(m_tRange), &(CObjMgr::Get_Instance()->Get_Player()->Get_Rect())))
	{
		CObj* pTarget = CObjMgr::Get_Instance()->Get_Instance()->Get_Player();

		m_tInfo.vDir = pTarget->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
	}


	Frame_Move();
	Scene_Change();

	return OBJ_NOEVENT;
}

void CSlime::LateUpdate_Object()
{

}

void CSlime::Render_Object()
{
	D3DXMATRIX	matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(L"SLIME", m_wstrStateKey, int(m_tFrame.dFrameStart));

	assert(!(pTexInfo == nullptr));

	float fCenterX = float(pTexInfo->tTexInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tTexInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	matWorld = matScale * matTrans;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	if (!m_bAlpha)
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	else
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 0, 0));


	// Rect
	m_tRect.left = (LONG)(m_tInfo.vPos.x - (fCenterX * 0.3f));
	m_tRect.top = (LONG)(m_tInfo.vPos.y - (fCenterY * 0.1f));
	m_tRect.right = (LONG)(m_tInfo.vPos.x + (fCenterX * 0.3f));
	m_tRect.bottom = (LONG)(m_tInfo.vPos.y + (fCenterY * 0.1f));



#ifdef _DEBUG
	// 충돌박스 출력
	D3DXVECTOR2 vLine[5] =
	{
		{ (float)m_tRect.left, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.top },
		{ (float)m_tRect.right, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.bottom },
		{ (float)m_tRect.left, (float)m_tRect.top }
	};

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);


	// 범위
	D3DXVECTOR2 vRangeLine[5] =
	{
		{ (float)m_tRange.left, (float)m_tRange.top },
		{ (float)m_tRange.right, (float)m_tRange.top },
		{ (float)m_tRange.right, (float)m_tRange.bottom },
		{ (float)m_tRange.left, (float)m_tRange.bottom },
		{ (float)m_tRange.left, (float)m_tRange.top }
	};

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vRangeLine, 5, D3DCOLOR_ARGB(255, 255, 240, 0));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif

}

void CSlime::Release_Object()
{
}

void CSlime::Frame_Move(const double& rFrameSpeed)
{
	m_tFrame.dFrameStart += m_tFrame.dFrameEnd * CTimeMgr::Get_Instance()->Get_DeltaTime() * rFrameSpeed;

	if (m_tFrame.dFrameStart >= m_tFrame.dFrameEnd)
	{
		if (m_eCurScene == IDLE)
		{
			m_eCurScene = JUMP;
			m_tFrame.dFrameStart = 0.0;
		}

		else if (m_eCurScene == JUMP)
		{
			m_eCurScene = IDLE;
			m_tFrame.dFrameStart = 0.0;
		}


		if (m_eCurScene == DEATH)
			m_bDead = true;

	}
}

void CSlime::Scene_Change()
{
	if (m_ePreScene != m_eCurScene)
	{
		switch (m_eCurScene)
		{
		case IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame = { 0.0, 9.0 };
			break;
		case JUMP:
			m_wstrStateKey = L"JUMP";
			m_tFrame = { 0.0, 11.0 };
			break;
		case DEATH:
			m_bAlpha = false;
			m_wstrStateKey = L"DEATH";
			m_tFrame = { 0.0, 10.0 };
			m_tInfo.vSize = { 1.f, 1.f, 0.f };
			break;
		}

		m_ePreScene = m_eCurScene;
	}
}
