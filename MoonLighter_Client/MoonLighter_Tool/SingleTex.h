#pragma once
#include "Texture.h"

class CSingleTex : public CTexture
{
public:
	CSingleTex();
	virtual ~CSingleTex();

public:
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"", const int& iCount = 0);

public:
	virtual HRESULT Insert_Texture(const wstring& wstrFilePath, const wstring& wstrStateKey = L"", const DWORD & dwCnt = 0) override;
	virtual void Release() override;

private:
	TEXINFO	m_tTexInfo;
};

