#include "stdafx.h"
#include "Graphic_Device.h"

CGraphic_Device* CGraphic_Device::m_pInstance = nullptr;


CGraphic_Device::CGraphic_Device()
{
}

CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}

HRESULT CGraphic_Device::Ready_Graphic_Device()
{
	D3DCAPS9	DeviceCap;
	ZeroMemory(&DeviceCap, sizeof(D3DCAPS9));

	// 1. 장치를 조사할 컴객체를 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// D3DADAPTER_DEFAULT - 진단 도구에 나열된 그래픽 카드를 사용하겠다
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCap)))
	{
		ERR_MSG(L"DeviceCaps Load Failed");
		return E_FAIL;
	}

	// 버텍스 프로세싱 = 정점변환 + 조명연산
	DWORD	dwvp = 0;	// 버텍스 프로세싱을 받아올 변수, 버텍스 프로세싱을 해주는지 안해주는지 검사

						// caps에 버텍스 프로세싱을 제공해주면 & 뒤에 비트 제공해줌
	if (DeviceCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwvp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// 하드웨어에서 버텍스 프로세싱
	else
		dwvp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// 소프트웨어에서 버텍스 프로세싱


	D3DPRESENT_PARAMETERS	d3dpp;	// 장치 세팅할 데이터를 담아둘 변수
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = WINCX;					// 백버퍼의 가로 길이
	d3dpp.BackBufferHeight = WINCY;					// 백버퍼의 세로 길이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		// 백버퍼의 포멧 형식
	d3dpp.BackBufferCount = 1;						// 백버퍼의 개수(기본 1개 + 대입한 값)
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	// 픽셀이 서로 부드럽게 이어져 보이도록 (안씀)
	d3dpp.MultiSampleQuality = 0;					// 얼마나 퀄리티를 살릴건지 (안씀)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 더블버퍼링과 비슷한 기능

													// 스왑체인
													// 전면버퍼와 후면버퍼를 두고 후면에서 그림을 그리고 전면 버퍼와 교체하는 방식.
													// 더블버퍼링은 전면과 후면이 있을 경우, 후면에 그림을 모두 그리고 전면버퍼에 복사하는 형식이었음

	d3dpp.hDeviceWindow = g_hWND;					// 그림을 그릴 곳 설정 - 윈도우창의 핸들을 넣어줌
	d3dpp.Windowed = TRUE;							// TRUE: 창모드, FALSE: 전체화면
	d3dpp.EnableAutoDepthStencil = TRUE;			// Depth와 Stencil 설정 (그려야 할 부분만 그리고 나머지 부분은 가려줌) - 지금 안씀(true)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Depth와 Stencil 어떻게 둘건지, Depth는 8비트, Stencil은 24비트
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// 풀스크린일때 어떻게 맞출지 (장치가 알아서)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// 게임과 화면을 어떤식으로 맞출건지


																	// 장치 세팅한 후 생성하기
																	// 어뎁트, 디바이스 타입, 주로 쓰는 윈도우(view창)의 핸들, 조사 수준, 내가 집어넣었던 것의 주소값, 컴객체의 주소값
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, dwvp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Device Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Sprite Create Failed");
		return E_FAIL;
	}

	// 폰트
	D3DXFONT_DESCW	 tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"D3DXFont Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Release_Graphic_Device()
{
	// 지우는 순서 !!
	// 자신이 참조하고 있는 것을 자신보다 뒤에서 지워야 한다.

	if (m_pSprite)
		m_pSprite->Release();

	// Sprite 안에서 Device를 참조하고 있으므로 Sprite 뒤에서 지운다.
	if (m_pDevice)
		m_pDevice->Release();

	// Device 안에서 SDK를 참조하고 있으므로 SDK를 뒤에서 지운다.
	if (m_pSDK)
		m_pSDK->Release();
}

void CGraphic_Device::Render_Begin()
{
	// 렌더링 과정: 후면버퍼를 지움 -> 후면버퍼에 그림 -> 후면버퍼와 전면 버퍼를 교체
	// 화면에 출력
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 0.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

// 그림을 그리고자 하는 대상을 인자로 받아온다
void CGraphic_Device::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	// 3번째 인자는 윈도우창 핸들. 지금은 null값으로 
	// -> 인자로 받아오는 윈도우창 핸들에 그린다
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
