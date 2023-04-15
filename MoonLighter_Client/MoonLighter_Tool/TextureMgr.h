#pragma once

class CTexture;

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	enum TEX_ID { TEX_SINGLE, TEX_MULTI };

public:
	const TEXINFO* Get_TexInfo(
		const wstring& wstrObjectKey,			// Object Ű��
		const wstring& wstrStateKey = L"",		// State Ű��. ��Ƽ �ؽ��ĸ�
		const DWORD& rCount = 0					// ��Ƽ�� ��� value��
	);

public:
	HRESULT Insert_Texture(
		TEX_ID eTexID,						// ��Ƽ���� �̱�����
		const wstring& wstrFilePath,		// ���� ���
		const wstring& wstrObjectKey,		// ������Ʈ Ű��
		const wstring& wstrStateKey = L"",	// ������Ʈ Ű��. ��Ƽ �ؽ��ĸ�
		const DWORD& rCount = 0				// ��Ƽ�� ��� value��
	);

public:
	void Release_TexMgr();

private:
	// ObjectKey, �̱��̳�, ��Ƽ�Ĵ� �θ� ������ �����̸� �� �� ���� �� ����
	map<wstring, CTexture*> m_mapTexture;
};

