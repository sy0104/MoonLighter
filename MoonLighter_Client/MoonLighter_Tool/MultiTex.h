#pragma once
#include "Texture.h"

class CMultiTex : public CTexture
{
public:
	explicit CMultiTex();
	virtual ~CMultiTex();

public:
	virtual const TEXINFO * Get_TexInfo(const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual HRESULT Insert_Texture(const wstring & wstrFilePath, const wstring & wstrStateKey = L"", const DWORD & dwCnt = 0) override;
	virtual void Release() override;

private:
	// StateKey - 행동
	// map이라는 객체는 배열을 가지고 있고
	// 그 배열을 찾아가는데 키값으로 찾아 갈것.
	// -> vector(동적배열)는 여러가지 그림을 가지고 있을 것.
	// 그리고 키값으로 내가 원하는 배열을 찾고
	// 원하는 그림을 인덱스로 꺼내 올것.
	map<wstring, vector<TEXINFO*>>	m_mapMultiTex;

	// 여러장의 그림을 벡터로 관리할 것임
	// -> value값을 벡터로 설정
	// -> map이라는 애가 관리하는건 vector(동적배열)
	// 이 vector는 키값으로 찾을 것임
};

