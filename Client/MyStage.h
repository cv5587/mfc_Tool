#pragma once
#include "Obj.h"
class CMyStage :
    public CObj
{
public:
	CMyStage();
	virtual ~CMyStage();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	
	void Set_ID(BYTE _id) {
		m_iDrawID = _id;
	}

	BYTE		m_iDrawID;
};

