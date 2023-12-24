#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "MainFrm.h"
#include "StageInfo.h"

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView*		pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CStageInfo* pStageInfo = pMainView->m_pStageInfo;
	CTerrain*		pTerrain = pMainView->m_pTerrain;
	pStageInfo->Mini_Render();
	pTerrain->Mini_Render();

	CDevice::Get_Instance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif 
