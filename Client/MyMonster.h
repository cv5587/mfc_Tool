#pragma once

#include "Obj.h"

class CMyMonster : public CObj
{
public:
	CMyMonster();
	virtual ~CMyMonster();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

	void		Load_Monster();

private:
	vector<STRUCT*> m_vecMonster;
};

