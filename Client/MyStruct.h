#pragma once
#include "Obj.h"
class CMyStruct :
    public CObj
{
public:
	CMyStruct();
	virtual ~CMyStruct();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	void		Load_Struct();

private:
	vector<STRUCT*>		m_vecStruct;
};

