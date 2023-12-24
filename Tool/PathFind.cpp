// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFind 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
	for_each(m_PathList.begin(), m_PathList.end(), CDeleteObj());
	m_PathList.clear();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind 메시지 처리기입니다.


void CPathFind::OnSaveData()
{
	wofstream		fout;

	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())// 개방 성공
	{
		for (auto& iter : m_PathList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnLoadData()
{
	UpdateData(TRUE);

	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt");

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());

		}

		fin.close();
	}

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPathFind::OnDropFiles(HDROP hDropInfo)
{

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	TCHAR	szFullPath[MAX_PATH] = L"";

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFullPath, m_PathList);
	}

	m_ListBox.ResetContent();

	wstring wstrCombined = L"";
	TCHAR	szBuf[MAX_STR] = L"";

	for (auto& iter : m_PathList)
	{
		// _itow_s : 정수를 문자열로 변환하는 함수
		_itow_s(iter->iCount, szBuf, 10);

		wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}
