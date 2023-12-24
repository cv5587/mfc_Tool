#pragma once

#include "Include.h"

class CDevice
{

	// 1. �׷��� ī�带 ������ ��ü ����
	// 2. �׷��� ī���� ������ ����(���� ����)
	// 3. �׷��� ī�带 ������ ��ü ����
		
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
	LPDIRECT3D9			m_pSDK;		// 1���� �ش��ϴ� ��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// 3���� �ش��ϴ� ��ü
	LPD3DXSPRITE		m_pSprite;	// dx �󿡼� 2d�̹����� ������ִ� com��ü
	LPD3DXFONT			m_pFont;	// dx �󿡼� ��Ʈ�� �̹��� ���·� ����ϴ� com��ü

};

