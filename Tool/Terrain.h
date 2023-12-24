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

// ������ �������� �̿��Ͽ� ���콺 �浹�� �����ؼ� Ÿ�� �̹����� �����ؿͶ�
// Y = AX + B	// A : ����	, B: Y ����