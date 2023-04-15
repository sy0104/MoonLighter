#include "stdafx.h"
#include "MultiTex.h"
#include "Graphic_Device.h"


CMultiTex::CMultiTex()
{
}


CMultiTex::~CMultiTex()
{
	Release();
}

const TEXINFO* CMultiTex::Get_TexInfo(const wstring & wstrStateKey, const int & iCount)
{
	auto& iter_find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() == iter_find)
		return nullptr;

	return iter_find->second[iCount];
}


HRESULT CMultiTex::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & dwCnt)
{
	auto& iter_Find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() != iter_Find)
		return E_FAIL;

	TCHAR		szFilePath[MAX_PATH] = L"";
	TEXINFO*	pTexInfo = nullptr;

	for (size_t i = 0; i < dwCnt; ++i)
	{
		wsprintf(szFilePath, wstrFilePath.c_str(), i);

		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->tTexInfo)))
		{
			wstring ERR = wstrFilePath + L"ImageLoad Failed";
			ERR_MSG(ERR.c_str());

			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(
			CGraphic_Device::Get_Instance()->Get_Device(),	// 디바이스 장치 요구
			szFilePath,							// 파일 경로
			pTexInfo->tTexInfo.Width,			// 그림의 가로 크기
			pTexInfo->tTexInfo.Height,			// 그림의 세로 크기
			pTexInfo->tTexInfo.MipLevels,		// 그림을 크기별로 미리 준비해 놓을 수 있음
			0,									// 그림의 사용 방식. 특별한게 없으면 0
			pTexInfo->tTexInfo.Format,			// 그림에 대한 픽셀 포멧
			D3DPOOL_MANAGED,					// 어디 메모리를 사용할지
			D3DX_DEFAULT,						// 이미지 필터
			D3DX_DEFAULT,						// 밉레벨을 어떻게 사용할지
			0,									// 없애고 싶은 색상
			nullptr,							// 이미지 정보
			nullptr,							// 팔레트
			&pTexInfo->pTexture)))				// 데이터를 집어넣을 공간
		{
			wstring wstrErr = wstrFilePath + L"Insert_Texture Failed";
			ERR_MSG(wstrErr.c_str());
			Safe_Delete(pTexInfo);

			return E_FAIL;
		}

		m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTex::Release()
{

	for (auto& rPair : m_mapMultiTex)
	{
		for (auto& pTexInfo : rPair.second)
		{
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
		}

		rPair.second.clear();
		rPair.second.swap(vector<TEXINFO*>());
	}

	m_mapMultiTex.clear();
}
