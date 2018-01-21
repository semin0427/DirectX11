//#include "stdafx.h"
#include "SuperClass.h"

CSuperClass::CSuperClass(ALL_INFO _tInfo)
	:m_tInfo(_tInfo)
{
}

CSuperClass::~CSuperClass(void)
{
}

bool CSuperClass::SetAtt(int _iAtt)
{
	m_tInfo.m_Hp -= _iAtt;
	if (m_tInfo.m_Hp <= 0)
	{
		m_tInfo.m_Hp = 0;
		m_tInfo.m_JGDeth = true;
		return true;
	}
	return false;
}

void CSuperClass::Release(void)
{
}