#pragma once

#include "Headers.h"

class CSuperClass
{
protected:
	CSuperClass(ALL_INFO _tInfo);
	~CSuperClass(void);

public:
	virtual void Update(void) = 0;
	virtual void Render(void) = 0;

public:
	ALL_INFO	GetINFO(void) { return m_tInfo; }
	void		GetInfo(ALL_INFO& _info) { _info = m_tInfo; }

public:
	void		SetINFO(const ALL_INFO& _tInfo) { m_tInfo = _tInfo; }
	bool		SetAtt(int _iAtt);

protected:
	ALL_INFO		m_tInfo;

public:
	void Release(void);

};