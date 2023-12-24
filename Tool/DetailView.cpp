#include "stdafx.h"
#include "Tool.h"
#include "DetailView.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CDetailView, CView)

CDetailView::CDetailView()
{
}

CDetailView::~CDetailView()
{
}

BEGIN_MESSAGE_MAP(CDetailView, CView)
END_MESSAGE_MAP()

void CDetailView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

#ifdef _DEBUG
void CDetailView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetailView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif
