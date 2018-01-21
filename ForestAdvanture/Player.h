#pragma once

#include "SuperClass.h"

class CPlayer :public CSuperClass
{
private:
	explicit CPlayer(const ALL_INFO& _tInfo);
	~CPlayer(void);

public:
	bool	Init(void);	

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CPlayer* Create(const ALL_INFO& _tInfo);

public:
	void move();

public:
	void		Release(void);
};
