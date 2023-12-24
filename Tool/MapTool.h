#pragma once
#include "afxwin.h"
#include <atlimage.h>
#include "Include.h"

// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
