#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "StructInfo.h"
#include "MonsterInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)

	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)

	ON_WM_DESTROY()

	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CToolView::CToolView()  : m_pTerrain(nullptr), m_pStageInfo(nullptr), m_pStructInfo(nullptr), m_bHide(false), m_pMonsterInfo(nullptr)
{
}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * (TILECY / 2)));

	CMainFrame*	pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Initialize()))
	{
		AfxMessageBox(L"Device Init Failed");
		return;
	}

	//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	//{
	//	AfxMessageBox(L"Cube Img Load Failed");
	//	return ;
	//}


	m_pStageInfo = new CStageInfo;
	m_pStageInfo->Initialize();
	m_pStageInfo->Set_MainView(this);

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	m_pStructInfo = new CStructInfo;	
	m_pStructInfo->Initialize();
	m_pStructInfo->Set_MainView(this);

	m_pMonsterInfo = new CMonsterInfo;
	m_pMonsterInfo->Initialize();
	m_pMonsterInfo->Set_MainView(this);

	opentool = TOOL_END;
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool* pMapTool = &(pMyForm->m_MapTool);	
		

	switch (opentool)
	{
	case TOOL_TILE:


		m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)),
			float(point.y + GetScrollPos(1)),
			0.f),
			pMapTool->m_iDrawID);

		Invalidate(FALSE);

		pMiniView->Invalidate(FALSE);
		break;
	case TOOL_MONSTER:
		if (!m_vinstanceVec.empty())
		{
			m_pMonsterInfo->Set_VecObj(m_vinstanceVec.front());

			CMonsterTool* pMonsterTool = &(pMyForm->m_MonsterTool);
			pMonsterTool->Set_Edit_XY(point.x, point.y);
			m_vinstanceVec.pop_back();
			Invalidate(FALSE);
		}
		else if (!m_vObjList.empty()) // 삭제 가능 대상이 있는 경우
		{
			int iCount(0);

			for (vector<STRUCT*>::iterator iter = m_vObjList.begin(); iter != m_vObjList.end();)
			{
				STRUCT* pObj = *iter;
				if (fabs(point.x - pObj->vPos.x) < 100.f && fabs(point.y - pObj->vPos.y) < 100.f)
				{
					m_vObjList.erase(iter);
					m_pMonsterInfo->Set_VecObjErase(iCount);
					break;
				}
				++iCount;
			}
			Invalidate(FALSE);
		}
		break;
	case TOOL_STRUCT:
		if (!m_vinstanceVec.empty())
		{
			m_pStructInfo->Set_VecObj(m_vinstanceVec.front());
			m_vinstanceVec.pop_back();
			Invalidate(FALSE);
		}
		break;
	default:
		break;
	}
	
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);

	switch (opentool)
	{
	case TOOL_TILE:
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
			CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
			CMapTool* pMapTool = &(pMyForm->m_MapTool);

			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x + GetScrollPos(0)),
				float(point.y + GetScrollPos(1)),
				0.f),
				pMapTool->m_iDrawID);

			Invalidate(FALSE);

			pMiniView->Invalidate(FALSE);
		}
		break;
	case TOOL_MONSTER:
		if (!m_vinstanceVec.empty())
		{
			m_vinstanceVec.front()->vPos = { D3DXVECTOR3(float(point.x + GetScrollPos(0)),
																							float(point.y + GetScrollPos(1)),
																							0.f) };
			Invalidate(FALSE);
		}
		break;
	case TOOL_STRUCT:
		if (!m_vinstanceVec.empty())
		{
			m_vinstanceVec.front()->vPos = { D3DXVECTOR3(float(point.x + GetScrollPos(0)),
																							float(point.y + GetScrollPos(1)),
																							0.f) };
			Invalidate(FALSE);
		}
		break;
	default:
		break;
	}

}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	CDevice::Get_Instance()->Render_Begin();
	m_pStageInfo->Render();
	if (!m_bHide)
	{
		m_pTerrain->Render();
	}
	
	m_pStructInfo->Render();
	m_pMonsterInfo->Render();
	Instance_Render();
	CDevice::Get_Instance()->Render_End();
}

void CToolView::OnDestroy()
{
	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pStageInfo);
	Safe_Delete(m_pStructInfo);

	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();

	CScrollView::OnDestroy();

}

void CToolView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 시간 나면 더블클릭하면 깔아둔 struct 선택 한후에 structtool 에서 delete 버튼을 생성한뒤 (누르면 삭제 되게 {벡터에서 삭제}) 제작하자

	CScrollView::OnLButtonDblClk(nFlags, point);
}
//123
void CToolView::Instance_Render()
{
	RECT	rc{};

	//GetClientRect : 현재 클라이언트 영역의 RECT 정보를 얻어옴
	GetClientRect(&rc);

	float		fX = WINCX / float(rc.right - rc.left);
	float		fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX	matWorld, matScale, matTrans;
	TCHAR	szBuf[MIN_STR] = L"";

	for (auto iter : m_vinstanceVec)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - this->GetScrollPos(0),	// 0은 x 스크롤 값 
			iter->vPos.y - this->GetScrollPos(1),	// 1은 y 스크롤 값	
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(iter->csObjKey.c_str(), iter->csStateKey.c_str(), iter->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(180, 255, 255, 255));
	}
}

void CToolView::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}


#pragma region 필요없는 것

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG

#pragma endregion 필요없는 것

