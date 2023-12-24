// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTest(_T(""))
	, m_strCopy(_T(""))
	, m_strName(_T(""))
	, m_iAttack(0)
	, m_iHp(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTest);
	DDX_Text(pDX, IDC_EDIT2, m_strCopy);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAttack);
	DDX_Text(pDX, IDC_EDIT5, m_iHp);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)

	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)

	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAdd)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnFindData)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);		// 다이얼로그 박스로부터 입력된 값들을 얻어옴

	m_strCopy = m_strTest;

	UpdateData(FALSE);		// 변수에 저장된 값들을 다이얼로그 박스에 반영
}


void CUnitTool::OnListBox()
{
	UpdateData(TRUE);

	CString		strFindName;

	// GetCurSel : 현재 리스트 박스에서 선택된 셀의 index 값을 반환
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 현재 인덱스에 해당하는 문자열을 listbox로 부터 얻어옴
	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnAdd()
{
	
	UpdateData(TRUE);

	UNITDATA*		pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iHp = m_iHp;

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;

	// AddString : 매개 변수에 해당하는 문자열을 리스트 박스에 등록
	m_ListBox.AddString(pUnit->strName);

	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), [](auto& MyPair) {

		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_mapUnitData.clear();



	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	HBITMAP	hBitMap = (HBITMAP)LoadImage(nullptr,
											L"../Texture/JusinLogo1.bmp",
											IMAGE_BITMAP, 
											100, 
											50, 
											LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	m_Bitmap.SetBitmap(hBitMap);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CUnitTool::OnFindData()
{
	UpdateData(TRUE);

	auto	iter = m_mapUnitData.find(m_strFindName);

	if (iter == m_mapUnitData.end())
		return;

	//FindString : 리스트 박스의 0번 목록부터 일치하는 인덱스를 찾아라
	int	iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	UpdateData(TRUE);

	CString		strFindName;

	// GetCurSel : 현재 리스트 박스에서 선택된 셀의 index 값을 반환
	int	iIndex = m_ListBox.GetCurSel();

	if (LB_ERR == iIndex)
		return;

	// GetText : 현재 인덱스에 해당하는 문자열을 listbox로 부터 얻어옴
	m_ListBox.GetText(iIndex, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_mapUnitData.erase(strFindName);

	m_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}

void CUnitTool::OnSaveData()
{
	// CFileDialog : 파일 열기 혹은 저장 작업에 필요한 대화 상자를 생성하는 객체
	CFileDialog		Dlg(FALSE,	  // true (불러오기), false(다른 이름으로 저장하기)
						L"dat",	  // 기본 확장자 명
						L"*.dat", // 대화 상자에 표시될 최초 파일명
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김 옵션) | OFN_OVERWRITEPROMPT(중복된 이름 파일 저장 시 경고 메세지 띄우기 옵션)
						L"Data Files(*.dat)|*.dat||", // 대화 상자에 표시될 파일 형식
						this);		// 부모 윈도우 주소


	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 파일 이름만 잘라내는 함수
	// 경로 상에 파일 이름이 없다면 제일 마지막 폴더명을 잘라내는 함수
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : 현재 다이얼로그가 선택한 경로를 반환
		CString			str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0), dwStrByte(0);

		for (auto& iter : m_mapUnitData)
		{
			// key 값 저장
			dwStrByte = sizeof(TCHAR) * (iter.first.GetLength() + 1);
			
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, iter.first.GetString(), dwStrByte, &dwByte, nullptr);

			// value 값 저장

			WriteFile(hFile, &(iter.second->byItem), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(iter.second->iHp), sizeof(int), &dwByte, nullptr);
		}

		CloseHandle(hFile);

	}
}


void CUnitTool::OnLoadData()
{
	UpdateData(TRUE);

	// CFileDialog : 파일 열기 혹은 저장 작업에 필요한 대화 상자를 생성하는 객체
	CFileDialog		Dlg(TRUE,	  // true (불러오기), false(다른 이름으로 저장하기)
		L"dat",	  // 기본 확장자 명
		L"*.dat", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기 전용 체크박스 숨김 옵션) | OFN_OVERWRITEPROMPT(중복된 이름 파일 저장 시 경고 메세지 띄우기 옵션)
		L"Data Files(*.dat)|*.dat||", // 대화 상자에 표시될 파일 형식
		this);		// 부모 윈도우 주소


	TCHAR	szPath[MAX_PATH] = L"";

	// 현재 프로젝트의 경로를 얻어오는 함수(절대 경로)
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 파일 이름만 잘라내는 함수
	// 경로 상에 파일 이름이 없다면 제일 마지막 폴더명을 잘라내는 함수
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		for (auto& iter : m_mapUnitData)
			delete iter.second;;

		m_mapUnitData.clear();

		// 리스트 박스의 목록 초기화
		m_ListBox.ResetContent();

		CString			str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0), dwStrByte(0);
		UNITDATA	tData{};

		while (true)
		{
			// key 값 로드
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*		pName = new TCHAR[dwStrByte];

			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value 로드

			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				pName = nullptr;
				break;
			}

			UNITDATA*	pUnit = new UNITDATA;
			pUnit->strName = pName;

			delete[]pName;
			pName = nullptr;

			pUnit->byItem = tData.byItem;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->iAttack = tData.iAttack;
			pUnit->iHp = tData.iHp;

			m_mapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);

		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}
