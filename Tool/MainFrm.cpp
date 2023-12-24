#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 350), pContext);
	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 250), pContext);

	m_MainSplitter.SetColumnInfo(0, 300, 10);

	return TRUE;
}
