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
	// StateKey - �ൿ
	// map�̶�� ��ü�� �迭�� ������ �ְ�
	// �� �迭�� ã�ư��µ� Ű������ ã�� ����.
	// -> vector(�����迭)�� �������� �׸��� ������ ���� ��.
	// �׸��� Ű������ ���� ���ϴ� �迭�� ã��
	// ���ϴ� �׸��� �ε����� ���� �ð�.
	map<wstring, vector<TEXINFO*>>	m_mapMultiTex;

	// �������� �׸��� ���ͷ� ������ ����
	// -> value���� ���ͷ� ����
	// -> map�̶�� �ְ� �����ϴ°� vector(�����迭)
	// �� vector�� Ű������ ã�� ����
};

