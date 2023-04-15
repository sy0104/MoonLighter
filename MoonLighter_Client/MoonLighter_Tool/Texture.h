#pragma once

class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

	// ObjectKey - ex) Player, Monster, NPC, Effect, Tile
	// StateKey(상태값) - ex) Player의 걷다, 뛰다, 때리다 등
	//					 ex) Monster의 걷다, 뛰다, 때리다 등

public:
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"", const int& iCount = 0) PURE;

public:
	virtual HRESULT Insert_Texture(
		const wstring& wstrFilePath,			// 경로
		const wstring& wstrStateKey = L"",		// 멀티만 사용
		const DWORD& dwCnt = 0) PURE;			// 그림이 몇장인지

	virtual void Release() PURE;
};


// Texture
// 싱글 텍스쳐: 단일 (그림이 한장인 것) - StateKey값이 없음
// 멀티 텍스쳐: 그림이 여러장 (이펙트)
