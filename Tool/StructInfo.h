#pragma once
#include "Include.h"
class CToolView;
class CStructInfo
{

public:
	CStructInfo();
	~CStructInfo();
public:
	void		Initialize(void);
	void		Update(void);
	void		Mini_Render(void);
	void		Render(void);
	void		Release(void);
	void Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
public:
	vector<STRUCT*>& Get_VecObj() { return m_vecObj; }	
	void Set_VecObj(STRUCT* _struct) {  m_vecObj.push_back(_struct); }	
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }


	
private:
	CToolView* m_pMainView;
	vector<STRUCT*>		m_vecObj;		
};

