#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3 vPos; // 위치백터
	D3DXVECTOR3 vSize; // 크기
	D3DXVECTOR3 vDir; // 단순 방향을 표시하기 위한 백터 
	D3DXVECTOR3 vLook; // 바라보는 방향이라고 하는데 그냥 기준 잡을라고 넣음. 
	D3DXMATRIX  matWorld;

}INFO;

typedef struct tagTexInfo
{
	PDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tTexInfo;
	// 이미지 정보를 보관할 구조체
	// 함수를 사용해서 데이터를 얻어올 것
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