//TODO:1220 맵 클래스 생성
#pragma once
#include"Include.h"
class CToolView;
class CStageInfo
{
public:

	CStageInfo();
	~CStageInfo();

public:
	void		Initialize(void);
	void		Update(void);
	void		Mini_Render(void);
	void		Render(void);
	void		Release(void);

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	BYTE		m_iDrawID;
	void		Stage_Change(BYTE id) { m_iDrawID = id; }
private:
	CToolView*		m_pMainView;


};

