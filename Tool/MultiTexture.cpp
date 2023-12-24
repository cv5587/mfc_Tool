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

		// D3DXGetImageInfoFromFile : ��ο� �ִ� �̹��� ���Ͽ� ���� ������ ���� ����ü�� �����ϴ� �Լ�
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
			0, // D3DUSAGE_RENDERTARGET(ȭ�� ��¿� �ؽ�ó)
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // �̹��� ���͸� ���, �̹��� Ȯ��, ��� �� ���ڶ� �ȼ��� ��� ä�� ���ΰ�
			D3DX_DEFAULT, // �Ӹ��� �̿��� ���͸� ���		
			0,			  // ������ ����, 0�� �÷�Ű ��ȿȭ
			NULL,		  // �̹��� ���� ���
			NULL,		  // �ȷ�Ʈ(8��Ʈ ���� ARGB ���� ǥ���ϱ� ���� ���� ����ü) ����
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
