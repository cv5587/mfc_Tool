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
	// 1. 그래픽 카드를 조사할 객체 생성
	// 2. 그래픽 카드의 성능을 조사(지원 수준)
	// 3. 그래픽 카드를 제어할 객체 생성

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보를 HAL에서부터 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻어려는 기본 그래픽 카드

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// MFC에서 제공하는 MESSAGEBOX 함수
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp(0);

	// HAL을 통해 조사한 장치가 하드웨어 버텍스 프로세싱을 지원한다면
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// 지원하지 않는다면
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	//CreateDevice : 장치를 제어할 객체를 생성하는 함수
	//CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보로 장치에 접근할 것인가, 장치 세팅을 반영할 윈도우 핸들, 동작 방식, 사용 환경, 생성한 컴객체 저장할 곳)
	
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	// 스프라이트 컴 객체 생성

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}
	
	// 폰트 컴 객체 생성

	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"D3DXCreateFontIndirect");
		return E_FAIL;
	}

	
	return S_OK;
}

void CDevice::Render_Begin()
{
	m_pDevice->Clear(0,		// RECT 배열의 원소 개수 
					nullptr,// 비율 영역에 해당하는 RECT 배열의 주소	
					D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 
					D3DCOLOR_ARGB(255, 125, 125, 125), // 후면 버퍼 색상					
					1.f, // z버퍼 초기화 값
					0);	 // 스텐실 버퍼 초기화 값

	// 여기서부터 후면 버퍼에 그리기를 시작함
	m_pDevice->BeginScene();

	// 매개 변수의 옵션은 알파테스트가 유효한 상태에서 알파 블랜딩을 사용하겠다는 옵션
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();

	// 후면 버퍼와 전면 버퍼가 가리키는 대상을 교환하여 화면에 출력
	// 1, 2번 인자 : 스왑 체인 옵션이 D3DSWAPEFFECT_COPY로 작성되지 않는한  NULL
	// 3번 인자 : 출력 대상 윈도우 핸들
	// 4번 인자 : 스왑 체인 옵션이 D3DSWAPEFFECT_COPY로 작성되지 않는한  NULL
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
	
	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나를 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// 초기화한 장치를 사용할 윈도우 핸들 지정
	d3dpp.hDeviceWindow = g_hWnd;

	d3dpp.Windowed = TRUE;		// 창 모드로 실행? 전체 화면 모드로 실행? (TRUE인 경우 창 모드로 실행)
	
	d3dpp.EnableAutoDepthStencil = TRUE;			// DX가 자동적으로 깊이 / 스텐실 버퍼 생성 후 관리
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	
	
	// 전체 화면 모드 일 경우에만 적용
	
	// 전체화면 모드 시, 화면 시연은 어떻게 설정 할 것인가
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// 전체 화면 모드 시, 현재 모니터 재생률을 토대로 알아서 수행

	// 모니터 재생률과 시연의 간격은 어떻게 할 것인가

	// D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 알아서 결정(모니터 재생률 따라감)

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
