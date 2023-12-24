#include "stdafx.h"
#include "StageInfo.h"
#include "TextureMgr.h"
#include "FileInfo.h"
#include "ToolView.h"
CStageInfo::CStageInfo()
	:m_iDrawID(0)
{
}

CStageInfo::~CStageInfo()
{
}

void CStageInfo::Initialize(void)
{	//TODO:1220 필드 삽입 
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/Stage/Map/Map%d.png", TEX_MULTI, L"Stage",L"Map",8)))
	{
		AfxMessageBox(L"Tile Img Load Failed");
		return;
	}
	
}

void CStageInfo::Update(void)
{
}

void CStageInfo::Mini_Render(void)
{
	D3DXMATRIX	matWorld, matScale, matTrans;
	TCHAR	szBuf[MIN_STR] = L"";

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		0.f - m_pMainView->GetScrollPos(0),	// 0은 x 스크롤 값 
		0.f - m_pMainView->GetScrollPos(1),	// 1은 y 스크롤 값
		0.f);

	matWorld = matScale * matTrans;


	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Map", m_iDrawID);

	Set_Ratio(&matWorld, 0.3f	, 0.3f);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStageInfo::Render(void)
{
	RECT	rc{};

	//GetClientRect : 현재 클라이언트 영역의 RECT 정보를 얻어옴
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float		fX = WINCX / float(rc.right - rc.left);
	float		fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX	matWorld, matScale, matTrans;	
	TCHAR	szBuf[MIN_STR] = L"";

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		0.f - m_pMainView->GetScrollPos(0),	// 0은 x 스크롤 값 
		0.f - m_pMainView->GetScrollPos(1),	// 1은 y 스크롤 값
		0.f);

	matWorld = matScale * matTrans;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Stage", L"Map", m_iDrawID);

	Set_Ratio(&matWorld, fX, fY);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStageInfo::Release(void)
{
}

void CStageInfo::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}
