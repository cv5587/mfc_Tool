// MonsterTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "MonsterTool.h"
#include "TextureMgr.h"
#include "MonsterInfo.h"
#include "MainFrm.h"


// CMonsterTool 대화 상자

IMPLEMENT_DYNAMIC(CMonsterTool, CDialog)

CMonsterTool::CMonsterTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMONSTERTOOL, pParent)
	, m_LevelEdit(_T(""))
	, m_HpEdit(_T(""))
	, m_AttackEdit(_T(""))
	, m_EditObjKey(_T(""))
	, m_EditStateKey(_T(""))
	, m_EditDrawID()
{

}

CMonsterTool::~CMonsterTool()
{
}

void CMonsterTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT11, m_LevelEdit);
	DDX_Text(pDX, IDC_EDIT7, m_HpEdit);
	DDX_Text(pDX, IDC_EDIT8, m_AttackEdit);
	DDX_Control(pDX, IDC_PICTURE7, m_Picture);
	DDX_Control(pDX, IDC_COMBO1, m_NameCBB);
	DDX_Control(pDX, IDC_LIST1, m_SpriteList);
	DDX_Control(pDX, IDC_EDIT5, m_EditX);
	DDX_Control(pDX, IDC_EDIT6, m_EditY);
	DDX_Text(pDX, IDC_EDIT1, m_EditObjKey);
	DDX_Text(pDX, IDC_EDIT2, m_EditStateKey);
	DDX_Text(pDX, IDC_EDIT3, m_EditDrawID);
	DDX_Control(pDX, IDC_CHECK1, m_bCheck);
}


BEGIN_MESSAGE_MAP(CMonsterTool, CDialog)

	ON_LBN_SELCHANGE(IDC_LIST1, &CMonsterTool::OnSelectSprite)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMonsterTool::OnSelectMst)
	ON_BN_CLICKED(IDC_BUTTON2, &CMonsterTool::OnDeleteData)
	ON_BN_CLICKED(IDOK, &CMonsterTool::OnSaveData)
	ON_BN_CLICKED(IDC_CHECK1, &CMonsterTool::OnHideTile)
	ON_BN_CLICKED(IDC_BUTTON1, &CMonsterTool::OnBnClickedButton1)	
END_MESSAGE_MAP()

BOOL CMonsterTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstBlueWolf/BlueWolf/BlueWolf%d.png", TEX_MULTI, L"MstBlueWolf", L"BlueWolf", 6)))
	{
		AfxMessageBox(L"Bluewolf Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstBoss/Boss/Boss%d.png", TEX_MULTI, L"MstBoss", L"Boss", 8)))
	{
		AfxMessageBox(L"Boss Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstCreedWizard/CreedWizard/CreedWizard%d.png", TEX_MULTI, L"MstCreedWizard", L"CreedWizard", 4)))
	{
		AfxMessageBox(L"CreedWizard Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstDarkCow/DarkCow/DarkCow%d.png", TEX_MULTI, L"MstDarkCow", L"DarkCow", 6)))
	{
		AfxMessageBox(L"DarkCow Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstJelly/Jelly/Jelly%d.png", TEX_MULTI, L"MstJelly", L"Jelly", 5)))
	{
		AfxMessageBox(L"Jelly Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstPinkWolf/PinkWolf/PinkWolf%d.png", TEX_MULTI, L"MstPinkWolf", L"PinkWolf", 6)))
	{
		AfxMessageBox(L"PinkWolf Img Load Failed");
		return TRUE;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/MstRedWolf/RedWolf/RedWolf%d.png", TEX_MULTI, L"MstRedWolf", L"RedWolf", 6)))
	{
		AfxMessageBox(L"RedWolf Img Load Failed");
		return TRUE;
	}

	CFileFind	Find;

	wstring		wstrFilePath = L"..\\Texture\\Talesweaver\\*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();
		if (Find.IsDots())
			continue;
		else
		{
			if (Find.IsSystem())
				continue;
			wstring ss = Find.GetFileTitle();
			wstring dd = L"M";
			if (ss.compare(0, 1, dd) != 0)
			{
				continue;
			}

			m_NameCBB.AddString(Find.GetFileTitle());
		}
	}

	return TRUE;
}


void CMonsterTool::OnSelectSprite()
{
	UpdateData(TRUE);

	TCHAR		tcFilePath[MAX_PATH] = L"..\\Texture\\Talesweaver\\Mst";
	CString		strFindName;
	TCHAR		tcFilename[MAX_PATH] = L"";

	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), tcFilename);
	// GetCurSel : 현재 리스트 박스에서 선택된 셀의 index 값을 반환
	int	iIndex = m_SpriteList.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 현재 인덱스에 해당하는 문자열을 listbox로 부터 얻어옴
	m_SpriteList.GetText(iIndex, strFindName);

	auto	iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	CString objkee = strFindName;

	m_EditObjKey = tcFilename;

	int t(0);
	int o(0);

	for (int i = 0; i < strFindName.GetLength(); ++i)
	{
		if (0 != isdigit(strFindName[i]))
		{
			o++;
		}
		else
		{
			t++;
		}
	}

	objkee.Delete(t, o);
	m_EditStateKey = objkee;
	strFindName.Delete(0, t);

	m_EditDrawID = _tstoi(strFindName);

	m_Picture.SetBitmap(*(iter->second));

	UpdateData(FALSE);
}


void CMonsterTool::OnSelectMst()
{
	UpdateData(TRUE);
	m_SpriteList.ResetContent();
	CFileFind		Find;


	TCHAR		tcFilePath[MAX_PATH] = L"..\\Texture\\Talesweaver";
	TCHAR		tcFilename[MAX_PATH] = L"";
	wstring		tcstatename = L"";
	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), tcFilename);
	PathCombine(tcFilePath, tcFilePath, tcFilename);

	tcstatename = (wstring)tcFilename;
	tcstatename.erase(0, 3);

	wstring		wstrFilePath = L"..\\" + (wstring)tcFilePath + L"\\" + tcstatename + L"\\*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();
		if (Find.IsDots())
			continue;
		else
		{
			if (Find.IsSystem())
				continue;
			m_SpriteList.AddString(Find.GetFileTitle());
			TCHAR CFilePath[MAX_PATH] = L"";
			lstrcpy(CFilePath, Find.GetFilePath().GetString());

			auto	iter = m_mapPngImg.find(Find.GetFileTitle());

			if (iter == m_mapPngImg.end())
			{
				CImage* pPngImg = new CImage;

				pPngImg->Load(CFilePath);

				m_mapPngImg.insert({ Find.GetFileTitle(), pPngImg });
			}
		}
	}
	UpdateData(FALSE);

}

//
//void CMonsterTool::OnSaveTile()
//{
//	STRUCT* CNewObj = new STRUCT;
//
//	CNewObj->vPos = { 0.f,0.f,0.f };
//
//	CString		strFindName;
//
//	int	iIndex = m_SpriteList.GetCurSel();
//
//	if (LB_ERR == iIndex)
//		return;
//
//	m_SpriteList.GetText(iIndex, strFindName);
//
//	CNewObj->byDrawID = Check_index(strFindName);
//	CString ee;
//	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), ee);
//
//	CNewObj->csObjKey = ee.GetString();
//
//	CString ss;
//	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), ss);
//	ss.Delete(0, 3);
//	//ee.Delete(0, 3);
//
//	CNewObj->csStateKey = ss;
//
//
//
//	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
//	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
//	pToolview->m_vinstanceVec.push_back(CNewObj);
//	pToolview->m_vObjList.push_back(CNewObj);
//}


void CMonsterTool::OnDeleteData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

CString CMonsterTool::ConvertRelativePath(CString strFullPath)
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CMonsterTool::DirInfoExtraction(list<IMGPATH*>& rPathList, int _Num)
{
	CFileFind Find;

	wstring wstrPath = L"";

	switch (_Num)
	{
	case BLUEWOLF:
		wstrPath = L"..\\Texture\\Talesweaver\\MstBlueWolf\\BlueWolf\\*.*";
		break;
	case CREEDWIZARD:
		wstrPath = L"..\\Texture\\Talesweaver\\MstCreedWizard\\CreedWizard\\*.*";
		break;
	case DARKCOW:
		wstrPath = L"..\\Texture\\Talesweaver\\MstDarkCow\\DarkCow\\*.*";
		break;
	case JELLY:
		wstrPath = L"..\\Texture\\Talesweaver\\MstJelly\\Jelly\\*.*";
		break;
	case PINKWOLF:
		wstrPath = L"..\\Texture\\Talesweaver\\MstPinkWolf\\PinkWolf\\*.*";
		break;
	case REDWOLF:
		wstrPath = L"..\\Texture\\Talesweaver\\MstRedWolf\\RedWolf\\*.*";
		break;
	case BOSS:
		wstrPath = L"..\\Texture\\Talesweaver\\MstBoss\\Boss\\*.*";
		break;
	default:
		break;
	}

	BOOL bContinue = Find.FindFile(wstrPath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		IMGPATH* pImgPath = new IMGPATH;
		TCHAR szPathBuf[MAX_PATH] = L"";

		lstrcpy(szPathBuf, Find.GetFilePath().GetString());
		PathRemoveFileSpec(szPathBuf);
		pImgPath->iCount = DirInfoCount(szPathBuf);

		wstring wstrTextureName = Find.GetFileTitle().GetString();

		wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size()) + L".png";

		TCHAR szBuf[MAX_PATH] = L"";
		lstrcpy(szBuf, szPathBuf);

		PathCombine(szBuf, szBuf, wstrTextureName.c_str());
		pImgPath->wstrPath = ConvertRelativePath(szBuf);

		PathRemoveFileSpec(szBuf);
		pImgPath->wstrStateKey = PathFindFileName(szBuf); // Stage Key

		PathRemoveFileSpec(szBuf);
		pImgPath->wstrObjKey = PathFindFileName(szBuf); // Object Key

		rPathList.push_back(pImgPath);
		//bContinue = 0;
	}
}

int CMonsterTool::DirInfoCount(const wstring& wstrPath)
{
	CFileFind Find;
	wstring wstrFilePath = wstrPath + L"\\*.*";
	BOOL bContinue = Find.FindFile(wstrFilePath.c_str());

	int iCount(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iCount;
	}
	return iCount;
}

BYTE CMonsterTool::Check_index(CString _strFindName)
{
	int i(0);

	for (; i < _strFindName.GetLength(); ++i)
	{
		if (0 != isdigit(_strFindName[i]))
			break;
	}

	_strFindName.Delete(0, i);

	// 문자를 정수형으로 변환
	return _tstoi(_strFindName);
}

void CMonsterTool::Set_Edit_XY(float _fX, float _fY)
{
	UpdateData(TRUE);

	CString str;

	str.Format(_T("%.f"), _fX);
	m_EditX.SetWindowText(str);

	str.Format(_T("%.f"), _fY);
	m_EditY.SetWindowTextW(str);

	UpdateData(FALSE);
}


void CMonsterTool::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	CMonsterInfo* pMonster = pToolView->m_pMonsterInfo;
	CStageInfo* pStage = pToolView->m_pStageInfo;
	vector<STRUCT*>& vecMonster = pMonster->Get_VecObj();
	BYTE stageid = pStage->m_iDrawID;
	wofstream fout;

	fout.open(L"../Data/MonsterPath.txt", ios::out);
	fout << stageid << endl;
	if (!fout.fail())
	{
		for (auto& iter : vecMonster)
		{
			fout << iter->csObjKey << L"|" << iter->csStateKey << L"|" 
				<< iter->byDrawID << L"|" << iter->vPos.x << L"|"<< iter->vPos.y << L"|" << iter->vPos.z << endl;
		}
		fout.close();
	}
}


void CMonsterTool::OnHideTile()
{
	UpdateData(TRUE);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	bool* p = &(pToolview->m_bHide);
	BOOL bChecked = m_bCheck.GetCheck();
	if (bChecked == TRUE)
	{
		*p = TRUE;
	}
	else
	{
		*p = FALSE;
	}
	pToolview->Invalidate(FALSE);

	UpdateData(FALSE);
}


void CMonsterTool::OnBnClickedButton1()
{
	STRUCT* CNewObj = new STRUCT;

	CNewObj->vPos = { 0.f,0.f,0.f };

	CString		strFindName;

	int	iIndex = m_SpriteList.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	m_SpriteList.GetText(iIndex, strFindName);

	CNewObj->byDrawID = Check_index(strFindName);
	CString ee;
	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), ee);

	CNewObj->csObjKey = ee.GetString();

	CString ss;
	m_NameCBB.GetLBText(m_NameCBB.GetCurSel(), ss);
	ss.Delete(0, 3);
	//ee.Delete(0, 3);

	CNewObj->csStateKey = ss;



	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->m_vinstanceVec.push_back(CNewObj);
	pToolview->m_vObjList.push_back(CNewObj);
}
