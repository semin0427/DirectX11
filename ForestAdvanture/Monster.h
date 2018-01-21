#pragma once

#include "SuperClass.h"

class CMonster :public CSuperClass
{
private:
	explicit CMonster(const ALL_INFO& _tInfo);
	~CMonster(void);

public:
	bool		Init(void);

public:
	virtual void Update(void);
	virtual void Render(void);


public:
	static CMonster* Create(const ALL_INFO& _tInfo);


public:
	void		Release(void);
};
