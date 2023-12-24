// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{
}

CMapTool::~CMapTool()
{
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), [](auto& MyPair) 
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});

	m_mapPngImg.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_CHECK1, m_bHide);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_BN_CLICKED(IDOK, &CMapTool::OnClose)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
END_MESSAGE_MAP()

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);


	TCHAR	szFilePath[MAX_PATH] = L"";
	TCHAR	szFileName[MAX_STR] = L"";

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		
		CString	strRelative = CFileInfo::ConvertRelativePath(szFilePath);

		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelative);
			
			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(szFileName);
		}		
	}

	Horizontal_Scroll();

	UpdateData(FALSE);

}

void CMapTool::Horizontal_Scroll()
{
	CString	strName;

	CSize		size;

	int		iWidth(0);

	CDC*	pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		size = pDC->GetTextExtent(strName);

		if (size.cx > iWidth)
			iWidth = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}

void CMapTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strFindName;

	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 현재 인덱스에 해당하는 문자열을 listbox로 부터 얻어옴
	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapPngImg.find(strFindName);

	if (iter == m_mapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i(0);

	for (; i < strFindName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 넘겨받은 단일 문자가 숫자 형태의 문자인지 글자 형태의 문자인지 구별하는 함수
		// 반환 값이 0이 아닌 값인 경우 숫자 타입의 문자를 의미
		if (0 != isdigit(strFindName[i]))
			break;
	}

	// Delete(index, count) : 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수 
	strFindName.Delete(0, i);

	// 문자를 정수형으로 변환
	m_iDrawID = _tstoi(strFindName);

	UpdateData(FALSE);
}

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCHAR	szFileName[MAX_STR] = L"";
	//list<CString> NameList;
	// 
	// 텍스쳐 받아오기
	DirInfoExtraction(m_ImgPathList);

	for (auto iter : m_ImgPathList)
	{
		// ConvertRelativePath
		CString strRelative = iter->wstrPath.c_str(); //
		CString strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName);

		strFileName = szFileName;

		auto filePath = m_mapPngImg.find(strFileName);

		if (filePath == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;

			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(strFileName);
		}
	}

	return TRUE;
}

CString CMapTool::ConvertRelativePath(CString strFullPath)
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

void CMapTool::DirInfoExtraction(list<IMGPATH*>& rPathList)
{
	CFileFind Find;

	wstring wstrPath = L"..\\Texture\\Talesweaver\\Terrain\\Tile\\*.*";

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

		//CString test = wstrTextureName.c_str(); // 읽어옴
		//rNameList.push_back(wstrTextureName.c_str()); // List에 push_back(출력용)

		// ListBox에 이름을 출력할 것이므로 필요 없는 코드
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

int CMapTool::DirInfoCount(const wstring& wstrPath)
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


void CMapTool::OnClose()
{
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pToolview = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	pToolview->opentool = TOOL_END;
	bool* p = &(pToolview->m_bHide);
	*p = FALSE;
	CDialog::OnOK();
}


void CMapTool::OnBnClickedCheck1()
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


void CMapTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,
		L"dat",
		L"*.dat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||",
		this);

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString			str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

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
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, 0);

		CloseHandle(hFile);

	}
}
