#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}

CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO * CMultiTexture::Get_Texture(const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_pMapMulti.begin(), m_pMapMulti.end(), [&](auto& MyPair)->bool
	{
		if (pStateKey == MyPair.first)
			return true;

		return false;
	});

	if (iter == m_pMapMulti.end())
		return nullptr;
	
	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey, const int & iCount)
{
	TCHAR	szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		// D3DXGetImageInfoFromFile : 경로에 있는 이미지 파일에 관한 정보를 얻어와 구조체에 저장하는 함수
		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(szFullPath);
			return E_FAIL;
		}

		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, // D3DUSAGE_RENDERTARGET(화면 출력용 텍스처)
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // 이미지 필터링 방식, 이미지 확대, 축소 시 모자란 픽셀을 어떻게 채울 것인가
			D3DX_DEFAULT, // 밉맵을 이용한 필터링 방식		
			0,			  // 제거할 색상, 0을 컬러키 무효화
			NULL,		  // 이미지 정보 기록
			NULL,		  // 팔레트(8비트 시절 ARGB 값을 표현하기 위해 만든 구조체) 정보
			&(pTexInfo->pTexture))))
		{
			Safe_Delete(pTexInfo);
			AfxMessageBox(L"MultiTexture Load Failed");
			return E_FAIL;
		}

		m_pMapMulti[pStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_pMapMulti.begin(), m_pMapMulti.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
		MyPair.second.clear();
	});

	m_pMapMulti.clear();
}
