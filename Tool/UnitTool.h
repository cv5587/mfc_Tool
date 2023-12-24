#pragma once
#include "afxwin.h"

#include "Include.h"

// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

// ����, �ҷ�����, ����(����Ʈ �ڽ����� ������ ������, ��ư ���� ��� ����), Ž��(edit control �Է��� ����)