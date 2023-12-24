// StructTool.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "StructTool.h"
#include "TextureMgr.h"
#include "StructInfo.h"
#include "MainFrm.h"

// CStructTool 대화 상자

IMPLEMENT_DYNAMIC(CStructTool, CDialog)

CStructTool::CStructTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CStructTool, pParent)
	, ObjKey(_T(""))
	, sStateKey(_T(""))
	, DrawID(0)
{


}

CStructTool::~CStructTool()
{
}

void CStructTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, StateKey);
	DDX_Control(pDX, IDC_LIST1, Filename);
	DDX_Control(pDX, IDC_PICTURE2, m_Picture);
	DDX_Text(pDX, IDC_EDIT1, ObjKey);
	DDX_Text(pDX, IDC_EDIT2, sStateKey);
	DDX_Text(pDX, IDC_EDIT3, DrawID);
	DDX_Control(pDX, IDC_CHECK1, m_bHide);
}


BEGIN_MESSAGE_MAP(CStructTool, CDialog)

	ON_BN_CLICKED(IDC_BUTTON1, &CStructTool::OnFileApply)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CStructTool::OnCbnChoice)
	ON_LBN_SELCHANGE(IDC_LIST1, &CStructTool::OnListBox)
	ON_BN_CLICKED(IDOK, &CStructTool::Ontoolclose)
	ON_BN_CLICKED(IDC_BUTTON2, &CStructTool::OnStructSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CStructTool::OnStructLoad)
	ON_BN_CLICKED(IDC_CHECK1, &CStructTool::OnHide)
END_MESSAGE_MAP()


// CStructTool 메시지 처리기


BYTE CStructTool::Check_index(CString		_strFindName)
{
	int i(0);

	for (; i < _strFindName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 넘겨받은 단일 문자가 숫자 형태의 문자인지 글자 형태의 문자인지 구별하는 함수
		// 반환 값이 0이 아닌 값인 경우 숫자 타입의 문자를 의미
		if (0 != isdigit(_strFindName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수 
	_strFindName.Delete(0, i);

	// 문자를 정수형으로 변환
	 return _tstoi(_strFindName);
}

void CStructTool::OnObjKey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CStructTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/ObjPlant/Plant/Plant%d.png", TEX_MULTI, L"ObjPlant", L"Plant", 19)))
	{
		AfxMessageBox(L"Plant Img Load Failed");
		return TRUE;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/ObjHouse/House/House%d.png", TEX_MULTI, L"ObjHouse", L"House", 4)))
	{
		AfxMessageBox(L"house Img Load Failed");
		return TRUE;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/ObjTree/Tree/Tree%d.png", TEX_MULTI, L"ObjTree", L"Tree", 50)))
	{
		AfxMessageBox(L"Tree Img Load Failed");
		return TRUE;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/ObjFence/Fence/Fence%d.png", TEX_MULTI, L"ObjFence", L"Fence", 16)))
	{
		AfxMessageBox(L"Fence Img Load Failed");
		return TRUE;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Talesweaver/ObjEtc/Etc/Etc%d.png", TEX_MULTI, L"ObjEtc", L"Etc", 38)))
	{
		AfxMessageBox(L"Etc Img Load Failed");
		return TRUE;
	}

	CFileFind		Find;

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
			wstring dd = L"O";

			if (ss.compare(0, 1, dd) != 0)
			{
				continue;
			}
			
			StateKey.AddString(Find.GetFileTitle());
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStructTool::OnFileApply()
{
	STRUCT* CObj = new STRUCT;	

	CObj->vPos = { 0.f,0.f,0.f };

	CString		strFindName;	

	int	iIndex = Filename.GetCurSel();	

	if (LB_ERR == iIndex)
		return;

	Filename.GetText(iIndex, strFindName);	

	CObj->byDrawID = Check_index(strFindName);	
	CString ee;
	StateKey.GetLBText(StateKey.GetCurSel(), ee);		


	CObj->csObjKey=ee.GetString();

	CString ss ;
	StateKey.GetLBText(StateKey.GetCurSel(), ss);		
	ss.Delete(0, 3);

	CObj->csStateKey = ss;



	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->m_vinstanceVec.push_back(CObj);
	//툴뷰에 넣어 두고 있을거양

}


void CStructTool::OnCbnChoice()
{
	UpdateData(TRUE);
	Filename.ResetContent();
	CFileFind		Find;


	TCHAR		tcFilePath[MAX_PATH] = L"..\\Texture\\Talesweaver";
	TCHAR		tcFilename[MAX_PATH] = L"";
	wstring		tcstatename = L"";
	StateKey.GetLBText(StateKey.GetCurSel(), tcFilename);
	PathCombine(tcFilePath, tcFilePath, tcFilename);
	
	tcstatename = (wstring)tcFilename;	
	tcstatename.erase(0, 3);

	wstring		wstrFilePath = L"..\\"+(wstring)tcFilePath + L"\\" + tcstatename + L"\\*.*";
	
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
			Filename.AddString(Find.GetFileTitle());
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


void CStructTool::OnListBox()
{
	UpdateData(TRUE);
	TCHAR		tcFilePath[MAX_PATH] = L"..\\Texture\\Talesweaver\\Obj";	
	CString		strFindName;	
	TCHAR		tcFilename[MAX_PATH] = L"";	
	StateKey.GetLBText(StateKey.GetCurSel(), tcFilename);			
	int	iIndex = Filename.GetCurSel();		

	if (LB_ERR == iIndex)
		return;

	Filename.GetText(iIndex, strFindName);	

	auto	iter = m_mapPngImg.find(strFindName);	

	if (iter == m_mapPngImg.end())	
		return;
	CString objkee =  strFindName;

	ObjKey = tcFilename;
	

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
	sStateKey = objkee;
	strFindName.Delete(0, t);

	DrawID = _tstoi(strFindName);

	m_Picture.SetBitmap(*(iter->second));	

	UpdateData(FALSE);
}


void CStructTool::Ontoolclose()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->opentool = TOOL_END;
	bool* p = &(pToolview->m_bHide);
	*p = FALSE;
	CDialog::OnOK();
}


void CStructTool::OnStructSave()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	CStructInfo* pStruct = pToolview->m_pStructInfo;	
	CStageInfo* pStage = pToolview->m_pStageInfo;		
	vector<STRUCT*> &  vecstruct= pStruct->Get_VecObj();		
	BYTE stageid=pStage->m_iDrawID;	
	wofstream		fout;

	fout.open(L"../Data/StructPath.txt", ios::out);
	fout << stageid << endl;
	if (!fout.fail())// 개방 성공
	{
		for (auto& iter : vecstruct)
		{
			fout << iter->csObjKey << L"|" << iter->csStateKey << L"|" << iter->byDrawID << L"|" << iter->vPos.x << L"|" << iter->vPos.y << L"|" << iter->vPos.z << endl;
		}

		fout.close();
	}

	//WinExec("notepad.exe ../Data/StructPath.txt", SW_SHOW);
}


void CStructTool::OnStructLoad()
{
	UpdateData(TRUE);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());	
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));	
	CStructInfo* pStructq = pToolview->m_pStructInfo;	
	vector<STRUCT*>& vecstruct = pStructq->Get_VecObj();		
	vecstruct.clear();

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

		BYTE 	bszCount;
		float fvx, fvy, fvz;

		wstring	wstrCombined = L"";

		STRUCT* pStruct = nullptr;


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

			pStructq->Set_VecObj(pStruct);

		}

		fin.close();
	}
	pToolview->Invalidate(FALSE);
	//WinExec("notepad.exe ../Data/StructPath.txt", SW_SHOW);
	UpdateData(FALSE);
}


void CStructTool::OnHide()
{
	UpdateData(TRUE);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	bool* p = &(pToolview->m_bHide);
	BOOL bChecked = m_bHide.GetCheck();	
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
