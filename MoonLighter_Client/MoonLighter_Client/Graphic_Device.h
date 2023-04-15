#pragma once

#ifndef __GRAPHICDEVICE_H__
#define __GRAPHICDEVICE_H__

class CGraphic_Device
{
private:
	CGraphic_Device();
	~CGraphic_Device();

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
	LPD3DXLINE		 Get_Line() { return m_pLine; }

public:
	HRESULT Ready_Graphic_Device();
	void Release_Graphic_Device();

public:
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	static CGraphic_Device*		m_pInstance;

	PDIRECT3D9			m_pSDK;
	PDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
	LPD3DXLINE			m_pLine;

};


#endif