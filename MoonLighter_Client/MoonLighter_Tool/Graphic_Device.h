#pragma once

class CGraphic_Device
{
private:
	CGraphic_Device();
	~CGraphic_Device();

	// �̱��� - ���������, ���Կ����ڵ� ������
	CGraphic_Device(const CGraphic_Device& rhs) {};
	void operator=(const CGraphic_Device& rhs) {};

public:
	static CGraphic_Device* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGraphic_Device;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	PDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPD3DXSPRITE	 Get_Sprite() { return m_pSprite; }
	LPD3DXFONT		 Get_Font() { return m_pFont; }

public:
	HRESULT Ready_Graphic_Device();
	void Release_Graphic_Device();

public:
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	static CGraphic_Device*		m_pInstance;

	// ����ϰ��� �ϴ� ��ġ�� �����ϰ� �ش� ��ġ�� �����ϱ� ���� ��ü�� �Ҵ��� ����ϴ� �İ�ü
	PDIRECT3D9			m_pSDK;
	// ���������� ��ġ�� �����ϴ� �İ�ü
	PDIRECT3DDEVICE9	m_pDevice;
	// �׸��� ����ϱ� ���� �İ�ü
	LPD3DXSPRITE		m_pSprite;
	// ��Ʈ�� ����ϴ� �İ�ü
	LPD3DXFONT			m_pFont;



	// component object model ��ü (�İ�ü)
	// 1. ��ġ�� ���� ������ ����
	// 2. ��ġ�� �´� ��ü�� ����
	// 3. �׸��׸���

	// DirectX��� ���� 3D�������� ���� API�� �����̴�.
	// �츮�� �׷��� ī�带 ���� ���ؼ� ���� �͵��� �ʿ��ϴ�.
	// �׷��� ���ҿ��� COM��ü��� ���� �غ�����.
	// �ϳ��� ��ǰ �������� �̹� ������ ���ش� ��� ��.
	// �̰� �̿��ؼ� ��ġ ���� �����ϵ� ���α׷��� �ϼ���Ű�� ��.
};