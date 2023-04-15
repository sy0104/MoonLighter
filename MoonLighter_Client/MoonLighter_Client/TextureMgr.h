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
		const wstring& wstrObjectKey,			// Object 키값
		const wstring& wstrStateKey = L"",		// State 키값. 멀티 텍스쳐만
		const DWORD& rCount = 0					// 멀티일 경우 value값
	);

public:
	HRESULT Insert_Texture(
		TEX_ID eTexID,						// 멀티인지 싱글인지
		const wstring& wstrFilePath,		// 파일 경로
		const wstring& wstrObjectKey,		// 오브젝트 키값
		const wstring& wstrStateKey = L"",	// 스테이트 키값. 멀티 텍스쳐만
		const DWORD& rCount = 0				// 멀티일 경우 value값
	);

public:
	HRESULT ReadImagePath(const wstring& wstrFilePath);

public:
	void Release_TexMgr();

private:
	map<wstring, CTexture*> m_mapTexture;
};

