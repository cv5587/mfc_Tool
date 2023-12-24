#pragma once

#include "Include.h"

class CDevice
{

	// 1. 그래픽 카드를 조사할 객체 생성
	// 2. 그래픽 카드의 성능을 조사(지원 수준)
	// 3. 그래픽 카드를 제어할 객체 생성
		
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font()	 { return m_pFont; }

public:
	HRESULT			Initialize(void);
	void			Render_Begin();
	void			Render_End(HWND hWnd = nullptr);
	void			Release();

private:
	void			Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// 1번에 해당하는 객체
	LPDIRECT3DDEVICE9	m_pDevice;	// 3번에 해당하는 객체
	LPD3DXSPRITE		m_pSprite;	// dx 상에서 2d이미지를 출력해주는 com객체
	LPD3DXFONT			m_pFont;	// dx 상에서 폰트를 이미지 형태로 출력하는 com객체

};

