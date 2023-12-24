#pragma once
#include "afxwin.h"

#include "Include.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnListBox();	
	afx_msg void OnAdd();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnFindData();
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();

public:
	// value
	CString m_strTest;
	CString m_strCopy;
	CString m_strName;
	CString m_strFindName;


	int m_iAttack;
	int m_iHp;

	// control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;

	// general
	map<CString, UNITDATA*>		m_mapUnitData;


};

// 저장, 불러오기, 삭제(리스트 박스에서 선택한 데이터, 버튼 누를 경우 삭제), 탐색(edit control 입력한 선택)