
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Device.h"
#include "TextureMgr.h"
#include "Terrain.h"
#include "StageInfo.h"
#include "StructInfo.h"
#include "MonsterInfo.h"

class CToolDoc;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	CTerrain*				m_pTerrain;
	CStageInfo*				m_pStageInfo;
	CStructInfo*			m_pStructInfo;
	CMonsterInfo*			m_pMonsterInfo;
	bool					m_bHide;

	vector<STRUCT*> m_vinstanceVec;
	vector<STRUCT*> m_vObjList;

	int					opentool;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void Instance_Render();
	void Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

