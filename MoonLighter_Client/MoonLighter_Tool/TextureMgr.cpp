#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTex.h"
#include "MultiTex.h"

IMPLEMENT_SINGLETON(CTextureMgr)


CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release_TexMgr();
}

const TEXINFO* CTextureMgr::Get_TexInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& rCount)
{
	// 1. �ʿ��� ������Ʈ Ű���� �˻�
	auto& iter_Fine = m_mapTexture.find(wstrObjectKey);

	// 2. ��ȸ�ߴµ� ��ã�Ҵٸ� nullptr ��ȯ
	if (iter_Fine == m_mapTexture.end())
		return nullptr;

	// 3. ã������ TexInfo ��ȯ
	return iter_Fine->second->Get_TexInfo(wstrStateKey, rCount);
}

HRESULT CTextureMgr::Insert_Texture(TEX_ID eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& rCount)
{
	// 1. �ʿ��� ������Ʈ Ű���� �˻�
	auto& iter_Find = m_mapTexture.find(wstrObjectKey);

	// 2. ���� ������ ���
	if (iter_Find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;

		switch (eTexID)
		{
			// �̱� �ؽ����� ���, StateKey���� �����Ƿ� ��������
		case CTextureMgr::TEX_SINGLE:
			pTexture = new CSingleTex;
			break;
		case CTextureMgr::TEX_MULTI:
			pTexture = new CMultiTex;
			break;
		default:
			break;
		}

		// ������ �̹����� ã�� ���� ���
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrERR = wstrStateKey + L"Insert Faield";
			ERR_MSG(wstrERR.c_str());

			return E_FAIL;
		}

		// �̱� �ؽ��� �߰�����
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}


	else if (TEX_MULTI == eTexID)
	{
		// ��Ƽ �ؽ����� ���, map�� �ε����� �����ؼ� �׾ȿ� ���Ҹ� �ִ� ���
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrErr = wstrStateKey + L"MultiTexture Insert_Failed";
			ERR_MSG(wstrErr.c_str());

			return E_FAIL;
		}
	}

	return S_OK;
}

void CTextureMgr::Release_TexMgr()
{
	for (auto& rPair : m_mapTexture)
	{
		Safe_Delete(rPair.second);
	}

	m_mapTexture.clear();
}
