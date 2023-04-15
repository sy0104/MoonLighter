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
	auto& iter_Fine = m_mapTexture.find(wstrObjectKey);

	if (iter_Fine == m_mapTexture.end())
		return nullptr;

	return iter_Fine->second->Get_TexInfo(wstrStateKey, rCount);
}

HRESULT CTextureMgr::Insert_Texture(TEX_ID eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& rCount)
{
	auto& iter_Find = m_mapTexture.find(wstrObjectKey);

	if (iter_Find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;

		switch (eTexID)
		{
		case CTextureMgr::TEX_SINGLE:
			pTexture = new CSingleTex;
			break;
		case CTextureMgr::TEX_MULTI:
			pTexture = new CMultiTex;
			break;
		default:
			break;
		}

		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrERR = wstrStateKey + L"Insert Faield";
			ERR_MSG(wstrERR.c_str());

			return E_FAIL;
		}

		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}


	else if (TEX_MULTI == eTexID)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrErr = wstrStateKey + L"MultiTexture Insert_Failed";
			ERR_MSG(wstrErr.c_str());

			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CTextureMgr::ReadImagePath(const wstring & wstrFilePath)
{
	wifstream fIn;
	fIn.open(wstrFilePath.c_str());

	if (!fIn.fail())
	{
		TCHAR	szFilePath[MAX_PATH] = L"";
		TCHAR	szObjectKey[MAX_PATH] = L"";
		TCHAR	szStateKey[MAX_PATH] = L"";
		TCHAR	szCount[MAX_PATH] = L"";

		while (true)
		{
			fIn.getline(szFilePath, MAX_PATH, L'|');
			fIn.getline(szObjectKey, MAX_PATH, L'|');
			fIn.getline(szStateKey, MAX_PATH, L'|');
			fIn.getline(szCount, MAX_PATH);

			int iCount = _ttoi(szCount);


			if (fIn.eof())
				break;

			if (FAILED(Insert_Texture(TEX_MULTI, szFilePath, szObjectKey, szStateKey, iCount)))
			{
				ERR_MSG(L"Texture Insert Failed");
				return E_FAIL;
			}
		}
	}

	fIn.close();
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
