#pragma once

class CTexture abstract
{
public:
	explicit CTexture();
	virtual ~CTexture();

	// ObjectKey - ex) Player, Monster, NPC, Effect, Tile
	// StateKey(���°�) - ex) Player�� �ȴ�, �ٴ�, ������ ��
	//					 ex) Monster�� �ȴ�, �ٴ�, ������ ��

public:
	virtual const TEXINFO* Get_TexInfo(const wstring& wstrStateKey = L"", const int& iCount = 0) PURE;

public:
	virtual HRESULT Insert_Texture(
		const wstring& wstrFilePath,			// ���
		const wstring& wstrStateKey = L"",		// ��Ƽ�� ���
		const DWORD& dwCnt = 0) PURE;			// �׸��� ��������

	virtual void Release() PURE;
};


// Texture
// �̱� �ؽ���: ���� (�׸��� ������ ��) - StateKey���� ����
// ��Ƽ �ؽ���: �׸��� ������ (����Ʈ)
