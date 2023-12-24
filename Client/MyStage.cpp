#include "stdafx.h"
#include "MyStage.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

CMyStage::CMyStage()
	:m_iDrawID(0)
{
}

CMyStage::~CMyStage()
{
    Release();
}

HRESULT CMyStage::Initialize(void)
{
	m_tInfo.vPos = { 0,0,0 };
	return S_OK;
}

int CMyStage::Update(void)
{
	if (0.f > ::Get_Mouse().x)
		m_vScroll.x += 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < ::Get_Mouse().x)
		m_vScroll.x -= 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > ::Get_Mouse().y)
		m_vScroll.y += 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < ::Get_Mouse().y)
		m_vScroll.y -= 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();


	return 0;
}

void CMyStage::Late_Update(void)
{
}

void CMyStage::Render(void)
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);	
			D3DXMatrixTranslation(&matTrans,	
				0.f + m_vScroll.x,	// 0인 경우 x 스크롤 값	
				0.f + m_vScroll.y,	// 1인 경우 y 스크롤 값	
				0.f);

			m_tInfo.matWorld = matScale * matTrans;		

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Map", m_iDrawID);
	
	if (nullptr == pTexInfo)
		return;


	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyStage::Release(void)
{
}


