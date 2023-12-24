#include "stdafx.h"
#include "StructInfo.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CStructInfo::CStructInfo()
{
}

CStructInfo::~CStructInfo()
{
	Release();
}

void CStructInfo::Initialize(void)
{
}

void CStructInfo::Update(void)
{
}

void CStructInfo::Mini_Render(void)
{
}

void CStructInfo::Render(void)
{
	RECT	rc{};

	//GetClientRect : 현재 클라이언트 영역의 RECT 정보를 얻어옴
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float		fX = WINCX / float(rc.right - rc.left);
	float		fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX	matWorld, matScale, matTrans;
	TCHAR	szBuf[MIN_STR] = L"";

	for (auto iter : m_vecObj)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),	// 0은 x 스크롤 값 
			iter->vPos.y - m_pMainView->GetScrollPos(1),	// 1은 y 스크롤 값	
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);	

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter->csObjKey.c_str(), iter->csStateKey.c_str(), iter->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CStructInfo::Release(void)
{
	for_each(m_vecObj.begin(), m_vecObj.end(), [](auto& p)
		{
			if (p)
			{
				delete p;
				p = nullptr;
			}
		});

	m_vecObj.clear();
}


void CStructInfo::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
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
