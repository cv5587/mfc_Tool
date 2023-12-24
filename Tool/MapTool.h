#pragma once
#include "afxwin.h"
#include <atlimage.h>
#include "Include.h"

// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnDropFiles(HDROP hDropInfo);
	afx_msg void	OnListBox();
	afx_msg void	OnClose();
	afx_msg void	OnBnClickedCheck1();
	afx_msg void	OnSaveData();

	void			Horizontal_Scroll();
	virtual BOOL	OnInitDialog();
	static CString		ConvertRelativePath(CString strFullPath);
	static void			DirInfoExtraction(list<IMGPATH*>& rPathList); // list<CString>& rNameList
	static int			DirInfoCount(const wstring& wstrPath);

public:
	CListBox					m_ListBox;
	CStatic						m_Picture;
	int							m_iDrawID = 0;
	
	map<CString, CImage*>		m_mapPngImg;
	CString						m_FileRoute;

	list<IMGPATH*>				m_ImgPathList;


	CButton m_bHide;

};
