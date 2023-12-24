#pragma once
#include "afxdialogex.h"
#include <atlimage.h>
#include "Include.h"

// CMonsterTool 대화 상자

class CMonsterTool : public CDialog
{
	DECLARE_DYNAMIC(CMonsterTool)

public:
	CMonsterTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMonsterTool();

enum MONSTER { BLUEWOLF, BOSS, CREEDWIZARD, DARKCOW, JELLY, PINKWOLF, REDWOLF };


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMONSTERTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectSprite();
	afx_msg void OnSelectMst();
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnHideTile();

	afx_msg void OnBnClickedButton1();

	static CString		ConvertRelativePath(CString strFullPath);
	static void			DirInfoExtraction(list<IMGPATH*>& rPathList, int _Num); // list<CString>& rNameList
	static int			DirInfoCount(const wstring& wstrPath);

	BYTE Check_index(CString _strFindName);
	void	Set_Edit_XY(float _fX, float _fY);
	

public:

	CString m_LevelEdit;
	CString m_HpEdit;
	CString m_AttackEdit;

	map<CString, CImage*>	m_mapPngImg;
	list<IMGPATH*>			m_ImgPathList;

	CStatic m_Picture;
	CComboBox m_NameCBB;
	CListBox m_SpriteList;

	CEdit m_EditX;
	CEdit m_EditY;
	CString m_EditObjKey;
	CString m_EditStateKey;
	BYTE m_EditDrawID;

	CButton m_bCheck;
};
