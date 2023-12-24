#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Initialize(void)
{
	// 1. �׷��� ī�带 ������ ��ü ����
	// 2. �׷��� ī���� ������ ����(���� ����)
	// 3. �׷��� ī�带 ������ ��ü ����

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ������ HAL�������� ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ ������ �⺻ �׷��� ī��

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// MFC���� �����ϴ� MESSAGEBOX �Լ�
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp(0);

	// HAL�� ���� ������ ��ġ�� �ϵ���� ���ؽ� ���μ����� �����Ѵٸ�
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// �������� �ʴ´ٸ�
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	//CreateDevice : ��ġ�� ������ ��ü�� �����ϴ� �Լ�
	//CreateDevice(� �׷��� ī�带 ������ ���ΰ�, � ������ ��ġ�� ������ ���ΰ�, ��ġ ������ �ݿ��� ������ �ڵ�, ���� ���, ��� ȯ��, ������ �İ�ü ������ ��)
	
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// ��������Ʈ �� ��ü ����

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}
	
	// ��Ʈ �� ��ü ����

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect");
		return E_FAIL;
	}

	
	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,		// RECT �迭�� ���� ���� 
					nullptr,// ���� ������ �ش��ϴ� RECT �迭�� �ּ�	
					D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 
					D3DCOLOR_ARGB(255, 125, 125, 125), // �ĸ� ���� ����					
					1.f, // z���� �ʱ�ȭ ��
					0);	 // ���ٽ� ���� �ʱ�ȭ ��

	// ���⼭���� �ĸ� ���ۿ� �׸��⸦ ������
	m_pDevice->BeginScene();

	// �Ű� ������ �ɼ��� �����׽�Ʈ�� ��ȿ�� ���¿��� ���� ������ ����ϰڴٴ� �ɼ�
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();

	// �ĸ� ���ۿ� ���� ���۰� ����Ű�� ����� ��ȯ�Ͽ� ȭ�鿡 ���
	// 1, 2�� ���� : ���� ü�� �ɼ��� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ���  NULL
	// 3�� ���� : ��� ��� ������ �ڵ�
	// 4�� ���� : ���� ü�� �ɼ��� D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ���  NULL
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDevice::Release()
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	
	if(nullptr != m_pDevice)
		m_pDevice->Release();

	if (nullptr != m_pSDK)
		m_pSDK->Release();

}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth	= WINCX;
	d3dpp.BackBufferHeight	= WINCY;
	d3dpp.BackBufferFormat	= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount	= 1;
	
	d3dpp.MultiSampleType		= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality	= 0;
	
	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// �ʱ�ȭ�� ��ġ�� ����� ������ �ڵ� ����
	d3dpp.hDeviceWindow = g_hWnd;

	d3dpp.Windowed = TRUE;		// â ���� ����? ��ü ȭ�� ���� ����? (TRUE�� ��� â ���� ����)
	
	d3dpp.EnableAutoDepthStencil = TRUE;			// DX�� �ڵ������� ���� / ���ٽ� ���� ���� �� ����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	
	
	// ��ü ȭ�� ��� �� ��쿡�� ����
	
	// ��üȭ�� ��� ��, ȭ�� �ÿ��� ��� ���� �� ���ΰ�
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ��ü ȭ�� ��� ��, ���� ����� ������� ���� �˾Ƽ� ����

	// ����� ������� �ÿ��� ������ ��� �� ���ΰ�

	// D3DPRESENT_INTERVAL_IMMEDIATE : ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ �˾Ƽ� ����(����� ����� ����)

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
