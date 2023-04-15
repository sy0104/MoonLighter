#pragma once

class CGraphic_Device
{
private:
	CGraphic_Device();
	~CGraphic_Device();

	// 싱글톤 - 복사생성자, 대입연산자도 막아줌
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

	// 사용하고자 하는 장치를 조사하고 해당 장치를 제어하기 위한 객체의 할당을 담당하는 컴객체
	PDIRECT3D9			m_pSDK;
	// 실질적으로 장치를 제어하는 컴객체
	PDIRECT3DDEVICE9	m_pDevice;
	// 그림을 출력하기 위한 컴객체
	LPD3DXSPRITE		m_pSprite;
	// 폰트를 출력하는 컴객체
	LPD3DXFONT			m_pFont;



	// component object model 객체 (컴객체)
	// 1. 장치의 제어 수준을 조사
	// 2. 장치에 맞는 객체를 생성
	// 3. 그림그릴꺼

	// DirectX라는 것은 3D렌더링을 위한 API의 일종이다.
	// 우리가 그래픽 카드를 쓰기 위해선 많은 것들이 필요하다.
	// 그래서 마소에서 COM객체라는 것을 준비해줌.
	// 하나의 부품 형식으로 이미 제공을 해준다 라는 것.
	// 이걸 이용해서 마치 레고 조립하듯 프로그램을 완성시키는 것.
};