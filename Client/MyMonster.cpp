#include "stdafx.h"
#include "MyMonster.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MyStage.h"

CMyMonster::CMyMonster()
{
}

CMyMonster::~CMyMonster()
{
	Release();
}

HRESULT CMyMonster::Initialize(void)
{
	Load_Monster();

	return S_OK;
}

int CMyMonster::Update(void)
{
	if (0.f > ::Get_Mouse().x)
		m_vScroll.x += 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < ::Get_Mouse().x)
		m_vScroll.x -= 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > ::Get_Mouse().y)
		m_vScroll.x += 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < ::Get_Mouse().y)
		m_vScroll.x -= 100.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	return 0;
}

void CMyMonster::Late_Update(void)
{
}

void CMyMonster::Render(void)
{
	D3DXMATRIX matWorld, matScale, matTrans;

	TCHAR szBuf[MIN_STR] = L"";
	int iIndex(0);

	int iCullX = int(-m_vScroll.x) / TILECX;
	int iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int iMaxX = WINCX / TILECX;
	int iMAxY = WINCY / (TILECY / 2);

	for (auto& iter : m_vecMonster)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x + m_vScroll.x,
			iter->vPos.y + m_vScroll.y,
			iter->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		TCHAR		szPathBuf[MAX_PATH] = L"";
		TCHAR		szqPathBuf[MAX_PATH] = L"";

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter->csObjKey.c_str(), iter->csStateKey.c_str(), iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CMyMonster::Release(void)
{
}

void CMyMonster::Load_Monster()
{
	wifstream fin;

	fin.open(L"../Data/MonsterPath.txt");

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	vx[MAX_STR] = L"";
		TCHAR	vy[MAX_STR] = L"";
		TCHAR	vz[MAX_STR] = L"";

		STRUCT* pMonsterStruct = nullptr;

		fin.getline(vz, MAX_PATH);
		dynamic_cast<CMyStage*>(CObjMgr::Get_Instance()->Get_Stage())->Set_ID(_tstoi(vz));
		while (true)
		{
			pMonsterStruct = new STRUCT;
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(vx, MAX_STR, '|');
			fin.getline(vy, MAX_STR, '|');
			fin.getline(vz, MAX_PATH);

			if (fin.eof())
				break;
			pMonsterStruct->csObjKey = (wstring)szObjKey;
			pMonsterStruct->csStateKey = (wstring)szStateKey;
			pMonsterStruct->byDrawID = _tstoi(szCount);
			pMonsterStruct->vPos.x = _tstoi(vx);
			pMonsterStruct->vPos.y = _tstoi(vy);
			pMonsterStruct->vPos.z = _tstoi(vz);


			m_vecMonster.push_back(pMonsterStruct);

		}

		fin.close();
	}
}