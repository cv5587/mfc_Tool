#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"



CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/MapTest.dat")))
	{
		ERR_MSG(L"Tile Data Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

int CMyTerrain::Update(void)
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

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iMaxX = WINCX / TILECX;
	int		iMaxY = WINCY / (TILECY / 2);

	for(int i = iCullY; i < iCullY + iMaxY; ++i)
	{ 
		for (int j = iCullX; j < iCullX + iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,	// 0인 경우 x 스크롤 값
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,	// 1인 경우 y 스크롤 값
				m_vecTile[iIndex]->vPos.z);

			m_tInfo.matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

			const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
				szBuf,
				lstrlen(szBuf),
				nullptr,
				0,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			++iIndex;
		}
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

HRESULT CMyTerrain::Load_Tile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	Release();

	DWORD		dwByte(0);
	TILE*		pTile = nullptr;


	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	
	return S_OK;
}
