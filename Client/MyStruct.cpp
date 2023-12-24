#include "stdafx.h"
#include "MyStruct.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MyStage.h"

CMyStruct::CMyStruct()
{
}

CMyStruct::~CMyStruct()
{
    Release();
}

HRESULT CMyStruct::Initialize(void)
{
	Load_Struct();

	return S_OK;
}

int CMyStruct::Update(void)
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

void CMyStruct::Late_Update(void)
{
}

void CMyStruct::Render(void)
{
	D3DXMATRIX		matWorld,matScale, matRotZ, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iMaxX = WINCX / TILECX;
	int		iMaxY = WINCY / (TILECY / 2);

	for (auto& iter : m_vecStruct)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x + m_vScroll.x,	// 0인 경우 x 스크롤 값
			iter->vPos.y + m_vScroll.y,	// 1인 경우 y 스크롤 값
			iter->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		TCHAR		szPathBuf[MAX_PATH] = L"";
		TCHAR		szqPathBuf[MAX_PATH] = L"";



		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter->csObjKey.c_str(), iter->csStateKey.c_str(), iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));


	}
		
	
}

void CMyStruct::Release(void)
{
}

void CMyStruct::Load_Struct()
{
		wifstream		fin;

		fin.open(L"../Data/StructPath.txt");

		if (!fin.fail())
		{
			TCHAR	szObjKey[MAX_STR] = L"";
			TCHAR	szStateKey[MAX_STR] = L"";
			TCHAR	szCount[MAX_STR] = L"";
			TCHAR	vx[MAX_STR] = L"";
			TCHAR	vy[MAX_STR] = L"";
			TCHAR	vz[MAX_STR] = L"";

			STRUCT* pStruct = nullptr;

			fin.getline(vz, MAX_PATH);	
			dynamic_cast<CMyStage*>(CObjMgr::Get_Instance()->Get_Stage())->Set_ID(_tstoi(vz));
			while (true)
			{
				pStruct = new STRUCT;	
				fin.getline(szObjKey, MAX_STR, '|');	
				fin.getline(szStateKey, MAX_STR, '|');
				fin.getline(szCount, MAX_STR, '|');
				fin.getline(vx, MAX_STR, '|');	
				fin.getline(vy, MAX_STR, '|');
				fin.getline(vz, MAX_PATH);

				if (fin.eof())
					break;
				pStruct->csObjKey = (wstring)szObjKey;
				pStruct->csStateKey = (wstring)szStateKey;
				pStruct->byDrawID = _tstoi(szCount);
				pStruct->vPos.x = _tstoi(vx);
				pStruct->vPos.y = _tstoi(vy);
				pStruct->vPos.z = _tstoi(vz);

				
				m_vecStruct.push_back(pStruct);

			}

			fin.close();
		}

		//WinExec("notepad.exe ../Data/StructPath.txt", SW_SHOW);
	

}
