//#include "stdafx.h"
#include "Monster.h"
#include "Defines.h"

CMonster::CMonster(const ALL_INFO& _tInfo)
	:CSuperClass(_tInfo)
{
}

CMonster::~CMonster(void)
{
}

bool CMonster::Init(void)
{
	return true;
}

void CMonster::Update(void)
{
	if (m_tInfo.m_JGDeth)
	{
		//공 , 방, 경험치, 현재피
		m_tInfo = Info(1, 1, 1, 1);
	}
}

void CMonster::Render(void)
{
}

CMonster* CMonster::Create(const ALL_INFO& _tInfo)
{
	CMonster* pInstance = new CMonster(_tInfo);

	/*if (!pInstance->Init())
		::Safe_Release(pInstance);*/

	return pInstance;
}

void CMonster::Release(void)
{
	delete this;
}