#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey, const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 경로에 있는 이미지 파일에 관한 정보를 얻어와 구조체에 저장하는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : dx 자원들에 대해 가장 적합한 메모리에 보관, 보통 그래픽 카드
	// D3DPOOL_MANAGED : dx에 의해 자원을 관리, 그래픽 카드 메모리를 사용하지만 이를 ram에 백업
	// D3DPOOL_SYSTEMMEM : 램에 보관
	// D3DPOOL_SCRATCH : 램에 보관하지만 dx장치가 접근할 수 없음
	
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
		pFilePath, 
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0, // D3DUSAGE_RENDERTARGET(화면 출력용 텍스처)
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, // 이미지 필터링 방식, 이미지 확대, 축소 시 모자란 픽셀을 어떻게 채울 것인가
		D3DX_DEFAULT, // 밉맵을 이용한 필터링 방식		
		0,			  // 제거할 색상, 0을 컬러키 무효화
		NULL,		  // 이미지 정보 기록
		NULL,		  // 팔레트(8비트 시절 ARGB 값을 표현하기 위해 만든 구조체) 정보
		&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"SingleTexture Load Failed");
		return E_FAIL;
	}


	return S_OK;
}

void CSingleTexture::Release()
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
