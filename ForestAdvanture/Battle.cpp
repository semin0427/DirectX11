#include "Battle.h"



CBattle::CBattle(void)
	:m_pPlayer(NULL),
	m_pMonster(NULL),
	m_pPlayerInit(NULL)
{
}

CBattle::~CBattle(void)
{
}

CBattle* CBattle::Create(void)
{
	CBattle* pInstance = new CBattle;

	return pInstance;
}

bool CBattle::Init(void)
{
	return true;
}

void CBattle::SetObject(CSuperClass* _pPlayer, CSuperClass* _pMonster)
{
	m_pPlayer = _pPlayer;
	m_pMonster = _pMonster;
}

void CBattle::Update()
{
		if (ATT(m_pMonster, m_pPlayer))
		{
			m_pMonster->Update();	//죽었을경우 초기화

			Sleep(2000);
			return;
		}

		if (ATT(m_pPlayer, m_pMonster))
		{

			m_pPlayer->Update();	//죽었을경우 초기화

			Sleep(2000);
			return;
		}

}

bool CBattle::ATT(CSuperClass* _pPlayer, CSuperClass* _pMonster)
{

	int iAtt = _pPlayer->GetINFO().m_Def - _pMonster->GetINFO().m_Att;

	if (iAtt >= 0)
		iAtt = 2;
	else
		iAtt *= -1;

	return _pPlayer->SetAtt(iAtt);
}


void CBattle::Release(void)
{
	delete this;
}