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

	// ���Ͽ��� �̹��� ���� �޾ƿ���
	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_tTexInfo.tTexInfo)))
	{
		wstring wstrErr = wstrFilePath + L"LoadFailed";
		ERR_MSG(wstrErr.c_str());

		return E_FAIL;
	}

	// �̹��� ���� �޾ƿ����� �̹��� �����ϴ� ��ü ����
	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphic_Device::Get_Instance()->Get_Device(),		// ����̽� ��ġ �䱸
		wstrFilePath.c_str(),			// ���� ���
		m_tTexInfo.tTexInfo.Width,		// �׸��� ���� ũ��, ������ ����
		m_tTexInfo.tTexInfo.Height,		// �׸��� ���� ũ��, ������ ����
		m_tTexInfo.tTexInfo.MipLevels,	// �׸��� ũ�⺰�� �̸� �غ��� ���� �� ����.
		0,								// �׸��� ��� ���. Ư���Ѱ� ������ 0
		m_tTexInfo.tTexInfo.Format,		// �׸��� ���� �ȼ� ����
		D3DPOOL_MANAGED,				// ��� �޸𸮸� ����� ���ΰ�. MANAGED�� �׷���ī���� �޸� ������ ����ϴٰ� �����ϸ� �ý��� �޸𸮵� ���
		D3DX_DEFAULT,					// �̹��� ����, �ִ� �״�� ��
		D3DX_DEFAULT,					// �ӷ����� ��� ����� ������, �ִ� �״��
		0,								// ���ְ� ���� ����. ���⼭�� ���� ������ ���� ����
		nullptr,						// �̹��� ���� - ������ ����
		nullptr,						// �ȷ�Ʈ - ���ſ� ���� ����. ����� �Ⱦ�
		&m_tTexInfo.pTexture			// PDIRECT3DTEXTURE9�� ������ ����. �� �����͸� ������� ����
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
