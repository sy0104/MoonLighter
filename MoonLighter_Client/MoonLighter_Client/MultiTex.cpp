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
			CGraphic_Device::Get_Instance()->Get_Device(),	// ����̽� ��ġ �䱸
			szFilePath,							// ���� ���
			pTexInfo->tTexInfo.Width,			// �׸��� ���� ũ��
			pTexInfo->tTexInfo.Height,			// �׸��� ���� ũ��
			pTexInfo->tTexInfo.MipLevels,		// �׸��� ũ�⺰�� �̸� �غ��� ���� �� ����
			0,									// �׸��� ��� ���. Ư���Ѱ� ������ 0
			pTexInfo->tTexInfo.Format,			// �׸��� ���� �ȼ� ����
			D3DPOOL_MANAGED,					// ��� �޸𸮸� �������
			D3DX_DEFAULT,						// �̹��� ����
			D3DX_DEFAULT,						// �ӷ����� ��� �������
			0,									// ���ְ� ���� ����
			nullptr,							// �̹��� ����
			nullptr,							// �ȷ�Ʈ
			&pTexInfo->pTexture)))				// �����͸� ������� ����
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
