// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "FileInfo.h"
#include "MainFrm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{
}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, StageSelect);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON10, &CMyForm::OnStructTool)
	ON_BN_CLICKED(IDC_BUTTON11, &CMyForm::OnMonsterTool)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMyForm::OnStageSelect)

	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnSaveData)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_Font.CreatePointFont(100, L"굴림");

	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON10)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON11)->SetFont(&m_Font);

	CFileFind		Find;

	wstring		wstrFilePath = L"..\\Texture\\Talesweaver\\Stage\\Map\\*.*";

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
			StageSelect.AddString(Find.GetFileTitle());
		}
	}
}

void CMyForm::OnUnitTool()
{
	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);

	m_UnitTool.ShowWindow(SW_SHOW);	
}


void CMyForm::OnMapTool()
{
	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->opentool = TOOL_TILE;	

	m_MapTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnPathFind()
{
	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);

	m_PathFind.ShowWindow(SW_SHOW);
}

void CMyForm::OnStructTool()
{
	if (nullptr == m_StructTool.GetSafeHwnd())
		m_StructTool.Create(IDD_CStructTool);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->opentool = TOOL_STRUCT;
	m_StructTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnMonsterTool()
{
	if (nullptr == m_MonsterTool.GetSafeHwnd())
		m_MonsterTool.Create(IDD_CMONSTERTOOL);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->opentool = TOOL_MONSTER;	
	m_MonsterTool.ShowWindow(SW_SHOW);
}

void CMyForm::OnStageSelect()
{
	UpdateData(TRUE);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));	
	CStageInfo* pStageSlect = pToolview->m_pStageInfo;
	pStageSlect->m_iDrawID = StageSelect.GetCurSel();

	pToolview->Invalidate(FALSE);

	pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMyForm::OnSaveData()
{
	CFileDialog Dlg(FALSE,
					L"dat",
					L"*.dat",
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					L"Data Files(*.dat)|*.dat||",
					this);

	TCHAR szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	// TODO: 타일 외의 object 들은 구조체를 다르게 해서 저장할 것인지?
	if (IDC_BUTTON2 == Dlg.DoModal())
	{
		CString			str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView* pTool = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
		CTerrain* pTerrain = pTool->m_pTerrain;
		vector<TILE*>& vecTile = pTerrain->Get_VecTile();
		if (vecTile.empty())
			return;

		DWORD dwByte(0);

		for (auto& iter : vecTile)
		{
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, 0);
		}

		CloseHandle(hFile);
	}
}
