#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3 vPos; // ��ġ����
	D3DXVECTOR3 vSize; // ũ��
	D3DXVECTOR3 vDir; // �ܼ� ������ ǥ���ϱ� ���� ���� 
	D3DXVECTOR3 vLook; // �ٶ󺸴� �����̶�� �ϴµ� �׳� ���� ������� ����. 
	D3DXMATRIX  matWorld;

}INFO;

typedef struct tagTexInfo
{
	PDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tTexInfo;
	// �̹��� ������ ������ ����ü
	// �Լ��� ����ؼ� �����͸� ���� ��
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;
	BYTE			byDrawID;
	BYTE			byOption;
}TILE;


typedef struct tagFrame
{
	double	dFrameStart;
	double	dFrameEnd;
}FRAME;

typedef struct tagFileInfo
{
	wstring		wstrFilePath;
	wstring		wstrObjectKey;
	wstring		wstrStateKey;

	int			iCount;
}FILEINFO;

typedef struct tagUnitInfo
{
	int		m_iAtt;
	int		m_iHp;
	int		m_iMaxHp;
}UNITINFO;