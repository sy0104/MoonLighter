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

const TEXINFO * CMultiTex::Get_TexInfo(const wstring & wstrStateKey, const int & iCount)
{
	// StateKey값으로 이미지가 있는지 없는지 찾기
	auto& iter_find = m_mapMultiTex.find(wstrStateKey);

	// 이미지가 없다면
	if (m_mapMultiTex.end() == iter_find)
		return nullptr;

	// 이미지가 있으면
	// value값인 second로 접근
	//return m_mapMultiTex[wstrStateKey][iCount];
	return iter_find->second[iCount];
}

//파일명이 다른 이미지들을 한번에 삽입하는 방법
// -> 문자열 대상으로 출력하는 함수 사용
// 화면에 출력하는게 아니라 첫번째로 받는 인자에 출력해줌
// swprintf_s();
// wsprintf();


HRESULT CMultiTex::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & dwCnt)
{
	auto& iter_Find = m_mapMultiTex.find(wstrStateKey);

	if (m_mapMultiTex.end() != iter_Find)
		return E_FAIL;

	TCHAR		szFilePath[MAX_PATH] = L"";
	TEXINFO*	pTexInfo = nullptr;

	for (int i = 0; i < dwCnt; ++i)
	{
		// szFilePath에 파일명을 넣어줌
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

		// 생성한 텍스쳐 맵에 삽입해줌
		m_mapMultiTex[wstrStateKey].emplace_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTex::Release()
{
	// Create로 데이터를 생성해 줬으니 Release도 해줘야 한다.

	for (auto& rPair : m_mapMultiTex)		// rPair: 맵에 있는 pair 객체
	{
		for (auto& pTexInfo : rPair.second)	// pTexInfo: 벡터의 배열에 있는 TexInfo* 인덱스
		{
			Safe_Release(pTexInfo->pTexture);
			Safe_Delete(pTexInfo);
		}

		rPair.second.clear();
		// vector는 clear를 해줘도 메모리 공간이 남아있기 때문에
		// 임시객체를 이용해 날려줘야 한다.
		rPair.second.swap(vector<TEXINFO*>());

		// shrink_to_fit(): 내가 가진 원소 개수만큼 메모리공간을 다시 잡아줌
		//rPair.second.shrink_to_fit();
	}

	m_mapMultiTex.clear();
}
