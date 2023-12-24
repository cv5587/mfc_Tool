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

	// D3DXGetImageInfoFromFile : ��ο� �ִ� �̹��� ���Ͽ� ���� ������ ���� ����ü�� �����ϴ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : dx �ڿ��鿡 ���� ���� ������ �޸𸮿� ����, ���� �׷��� ī��
	// D3DPOOL_MANAGED : dx�� ���� �ڿ��� ����, �׷��� ī�� �޸𸮸� ��������� �̸� ram�� ���
	// D3DPOOL_SYSTEMMEM : ���� ����
	// D3DPOOL_SCRATCH : ���� ���������� dx��ġ�� ������ �� ����
	
	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
		pFilePath, 
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0, // D3DUSAGE_RENDERTARGET(ȭ�� ��¿� �ؽ�ó)
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, // �̹��� ���͸� ���, �̹��� Ȯ��, ��� �� ���ڶ� �ȼ��� ��� ä�� ���ΰ�
		D3DX_DEFAULT, // �Ӹ��� �̿��� ���͸� ���		
		0,			  // ������ ����, 0�� �÷�Ű ��ȿȭ
		NULL,		  // �̹��� ���� ���
		NULL,		  // �ȷ�Ʈ(8��Ʈ ���� ARGB ���� ǥ���ϱ� ���� ���� ����ü) ����
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
