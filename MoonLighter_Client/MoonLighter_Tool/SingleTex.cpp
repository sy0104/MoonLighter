#include "stdafx.h"
#include "SingleTex.h"
#include "Graphic_Device.h"

CSingleTex::CSingleTex()
{
}


CSingleTex::~CSingleTex()
{
	Release();
}

const TEXINFO* CSingleTex::Get_TexInfo(const wstring & wstrStateKey, const int & iCount)
{
	return &m_tTexInfo;
}

HRESULT CSingleTex::Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey, const DWORD & dwCnt)
{
	ZeroMemory(&m_tTexInfo, sizeof(TEXINFO));

	// 파일에서 이미지 정보 받아오기
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tTexInfo)))
	{
		wstring wstrErr = wstrFilePath + L"LoadFailed";
		ERR_MSG(wstrErr.c_str());

		return E_FAIL;
	}

	// 이미지 정보 받아왔으면 이미지 제어하는 객체 생성
	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphic_Device::Get_Instance()->Get_Device(),		// 디바이스 장치 요구
		wstrFilePath.c_str(),			// 파일 경로
		m_tTexInfo.tTexInfo.Width,		// 그림의 가로 크기, 위에서 얻음
		m_tTexInfo.tTexInfo.Height,		// 그림의 세로 크기, 위에서 얻음
		m_tTexInfo.tTexInfo.MipLevels,	// 그림을 크기별로 미리 준비해 놓을 수 있음.
		0,								// 그림의 사용 방식. 특별한게 없으면 0
		m_tTexInfo.tTexInfo.Format,		// 그림에 대한 픽셀 포멧
		D3DPOOL_MANAGED,				// 어디 메모리를 사용할 것인가. MANAGED는 그래픽카드의 메모리 공간을 사용하다가 부족하면 시스템 메모리도 사용
		D3DX_DEFAULT,					// 이미지 필터, 있는 그대로 씀
		D3DX_DEFAULT,					// 밉레벨은 어떻게 사용할 것인지, 있는 그대로
		0,								// 없애고 싶은 색상. 여기서는 색상 없애지 않을 것임
		nullptr,						// 이미지 정보 - 위에서 얻어옴
		nullptr,						// 팔레트 - 과거에 쓰던 인자. 현재는 안씀
		&m_tTexInfo.pTexture			// PDIRECT3DTEXTURE9의 포인터 형식. 즉 데이터를 집어넣을 공간
	)))
	{
		wstring wstrErr = wstrFilePath + L"Insert_Texture Failed";
		ERR_MSG(wstrErr.c_str());

		return E_FAIL;
	}


	return S_OK;
}

void CSingleTex::Release()
{
	if (m_tTexInfo.pTexture)
	{
		m_tTexInfo.pTexture->Release();
	}
}
