#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize(void);
	void		Update(void);
	void		Mini_Render(void);
	void		Render(void);
	void		Release(void);

public:
	vector<TILE*>&		Get_VecTile() { return m_vecTile; }

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	int			GetTileIndex(const D3DXVECTOR3& vPos);

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	CToolView*			m_pMainView;
	vector<TILE*>		m_vecTile;
};

// 직선의 방정식을 이용하여 마우스 충돌을 구현해서 타일 이미지를 변경해와라
// Y = AX + B	// A : 기울기	, B: Y 절편