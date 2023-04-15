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
	// 1. 맵에서 오브젝트 키값을 검색
	auto& iter_Fine = m_mapTexture.find(wstrObjectKey);

	// 2. 순회했는데 못찾았다면 nullptr 반환
	if (iter_Fine == m_mapTexture.end())
		return nullptr;

	// 3. 찾았으면 TexInfo 반환
	return iter_Fine->second->Get_TexInfo(wstrStateKey, rCount);
}

HRESULT CTextureMgr::Insert_Texture(TEX_ID eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& rCount)
{
	// 1. 맵에서 오브젝트 키값을 검색
	auto& iter_Find = m_mapTexture.find(wstrObjectKey);

	// 2. 최초 삽입일 경우
	if (iter_Find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;

		switch (eTexID)
		{
			// 싱글 텍스쳐일 경우, StateKey값이 없으므로 생성해줌
		case CTextureMgr::TEX_SINGLE:
			pTexture = new CSingleTex;
			break;
		case CTextureMgr::TEX_MULTI:
			pTexture = new CMultiTex;
			break;
		default:
			break;
		}

		// 삽입할 이미지를 찾지 못한 경우
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrERR = wstrStateKey + L"Insert Faield";
			ERR_MSG(wstrERR.c_str());

			return E_FAIL;
		}

		// 싱글 텍스쳐 추가해줌
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}


	else if (TEX_MULTI == eTexID)
	{
		// 멀티 텍스쳐인 경우, map의 인덱스로 접근해서 그안에 원소를 넣는 방식
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
