#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"
#include "MyStage.h"
#include "MyStruct.h"
#include "MyMonster.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{

	if (FAILED(CTextureMgr::Get_Instance()->Read_ImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"Read_ImgPath Failed");
		return E_FAIL;
	}

	CObj* pObj;
	pObj= new CMyStage;	
	pObj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TALESSTAGE, pObj);

	pObj = new CMyTerrain;

	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pObj);

	pObj = new CMyStruct;
	pObj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::STRUCT, pObj);

	pObj = new CMyMonster;
	pObj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(CObjMgr::MONSTER, pObj);


	// 플레이어
	/*pObj = new CPlayer;
	if (nullptr == pObj)
		return E_FAIL;

	pObj->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pObj);*/

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
}
