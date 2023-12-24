#pragma once

#include "Include.h"

class CToolView;
class CMonsterInfo
{
public:
	CMonsterInfo();
	~CMonsterInfo();

public:
	void	Initialize();
	void	Update();
	void	Render();
	void	Release();

	void	Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void	Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void	Set_VecObj(STRUCT* _struct) { m_vecObj.push_back(_struct); }
	void	Set_VecObjErase(int _iCount);
	vector<STRUCT*>& Get_VecObj() { return m_vecObj; }

public:



private:
	CToolView* m_pMainView;
	vector<STRUCT*> m_vecObj;

};

