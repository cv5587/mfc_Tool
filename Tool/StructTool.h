#pragma once
#include "afxdialogex.h"
#include <atlimage.h>
#include "Include.h"

// CStructTool 대화 상자

class CStructTool : public CDialog
{
	DECLARE_DYNAMIC(CStructTool)

public:
	CStructTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CStructTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CStructTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	BYTE Check_index(CString		_strFindName);
public:

	afx_msg void OnObjKey();
	afx_msg void OnFileApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnChoice();
	afx_msg void OnListBox();
	afx_msg void Ontoolclose();



	CComboBox StateKey;
	CListBox Filename;

	map<CString, CImage*>		m_mapPngImg;	

	CStatic m_Picture;

	CString ObjKey;
	CString sStateKey;
	BYTE		 DrawID;
	afx_msg void OnStructSave();
	afx_msg void OnStructLoad();
	afx_msg void OnHide();
	CButton m_bHide;	
};
