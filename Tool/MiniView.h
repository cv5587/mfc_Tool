#pragma once

#include "Device.h"

class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

protected:
	CMiniView(); 
	virtual ~CMiniView();

public:
	virtual void OnDraw(CDC* pDC);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


