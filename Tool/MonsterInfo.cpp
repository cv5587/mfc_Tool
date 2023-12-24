#include "stdafx.h"
#include "MonsterInfo.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CMonsterInfo::CMonsterInfo()
{
}

CMonsterInfo::~CMonsterInfo()
{
	Release();
}

void CMonsterInfo::Initialize()
{
}

void CMonsterInfo::Update()
{
}

void CMonsterInfo::Render()
{
	RECT rc{};

	GetClientRect(m_pMainView->m_hWnd, &rc);

	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX matWorld, matScale, matTrans;
	TCHAR szBuf[MIN_STR] = L"";

	for (auto iter : m_vecObj)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0),
			iter->vPos.y - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter->csObjKey.c_str(), iter->csStateKey.c_str(), iter->byDrawID);

		float fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CMonsterInfo::Release()
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

void CMonsterInfo::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
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

void CMonsterInfo::Set_VecObjErase(int _iCount)
{
	vector<STRUCT*>::iterator iter = m_vecObj.begin();

	for (int i = 0; i < _iCount; ++i)
	{
		++iter;
	}
	m_vecObj.erase(iter);
}
