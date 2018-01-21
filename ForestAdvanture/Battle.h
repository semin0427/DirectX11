#pragma once
#include "Defines.h"

class CSuperClass;
class CPlayer;
class CBattle
{
public:
	CBattle(void);
	~CBattle(void);

public:
	void SetObject(CSuperClass* _pPlayer, CSuperClass* _pMonster);

public:
	void Update(void);
	static CBattle* Create(void);

public:
	bool ATT(CSuperClass* _pPlayer, CSuperClass* _pMonster);
	bool Init(void);

private:
	CSuperClass* m_pPlayer;
	CSuperClass* m_pMonster;
	CPlayer*	m_pPlayerInit;

public:
	void Release(void);
};

