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

	// 1. ��ġ�� ������ �İ�ü�� ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// D3DADAPTER_DEFAULT - ���� ������ ������ �׷��� ī�带 ����ϰڴ�
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCap)))
	{
		ERR_MSG(L"DeviceCaps Load Failed");
		return E_FAIL;
	}

	// ���ؽ� ���μ��� = ������ȯ + ������
	DWORD	dwvp = 0;	// ���ؽ� ���μ����� �޾ƿ� ����, ���ؽ� ���μ����� ���ִ��� �����ִ��� �˻�

						// caps�� ���ؽ� ���μ����� �������ָ� & �ڿ� ��Ʈ ��������
	if (DeviceCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwvp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// �ϵ����� ���ؽ� ���μ���
	else
		dwvp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// ����Ʈ����� ���ؽ� ���μ���


	D3DPRESENT_PARAMETERS	d3dpp;	// ��ġ ������ �����͸� ��Ƶ� ����
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = WINCX;					// ������� ���� ����
	d3dpp.BackBufferHeight = WINCY;					// ������� ���� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		// ������� ���� ����
	d3dpp.BackBufferCount = 1;						// ������� ����(�⺻ 1�� + ������ ��)
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	// �ȼ��� ���� �ε巴�� �̾��� ���̵��� (�Ⱦ�)
	d3dpp.MultiSampleQuality = 0;					// �󸶳� ����Ƽ�� �츱���� (�Ⱦ�)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// ������۸��� ����� ���

													// ����ü��
													// ������ۿ� �ĸ���۸� �ΰ� �ĸ鿡�� �׸��� �׸��� ���� ���ۿ� ��ü�ϴ� ���.
													// ������۸��� ����� �ĸ��� ���� ���, �ĸ鿡 �׸��� ��� �׸��� ������ۿ� �����ϴ� �����̾���

	d3dpp.hDeviceWindow = g_hWND;					// �׸��� �׸� �� ���� - ������â�� �ڵ��� �־���
	d3dpp.Windowed = TRUE;							// TRUE: â���, FALSE: ��üȭ��
	d3dpp.EnableAutoDepthStencil = TRUE;			// Depth�� Stencil ���� (�׷��� �� �κи� �׸��� ������ �κ��� ������) - ���� �Ⱦ�(true)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Depth�� Stencil ��� �Ѱ���, Depth�� 8��Ʈ, Stencil�� 24��Ʈ
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// Ǯ��ũ���϶� ��� ������ (��ġ�� �˾Ƽ�)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// ���Ӱ� ȭ���� ������� �������


																	// ��ġ ������ �� �����ϱ�
																	// �Ʈ, ����̽� Ÿ��, �ַ� ���� ������(viewâ)�� �ڵ�, ���� ����, ���� ����־��� ���� �ּҰ�, �İ�ü�� �ּҰ�
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

	// ��Ʈ
	D3DXFONT_DESCW	 tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"D3DXFont Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Release_Graphic_Device()
{
	// ����� ���� !!
	// �ڽ��� �����ϰ� �ִ� ���� �ڽź��� �ڿ��� ������ �Ѵ�.

	if (m_pSprite)
		m_pSprite->Release();

	// Sprite �ȿ��� Device�� �����ϰ� �����Ƿ� Sprite �ڿ��� �����.
	if (m_pDevice)
		m_pDevice->Release();

	// Device �ȿ��� SDK�� �����ϰ� �����Ƿ� SDK�� �ڿ��� �����.
	if (m_pSDK)
		m_pSDK->Release();
}

void CGraphic_Device::Render_Begin()
{
	// ������ ����: �ĸ���۸� ���� -> �ĸ���ۿ� �׸� -> �ĸ���ۿ� ���� ���۸� ��ü
	// ȭ�鿡 ���
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 0.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

// �׸��� �׸����� �ϴ� ����� ���ڷ� �޾ƿ´�
void CGraphic_Device::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	// 3��° ���ڴ� ������â �ڵ�. ������ null������ 
	// -> ���ڷ� �޾ƿ��� ������â �ڵ鿡 �׸���
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
