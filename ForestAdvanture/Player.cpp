//#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(const ALL_INFO& _tInfo)
	:CSuperClass(_tInfo)
{
}

CPlayer::~CPlayer(void)
{
}


bool CPlayer::Init(void)
{
	return true;
}

CPlayer* CPlayer::Create(const ALL_INFO& _tInfo)
{
	CPlayer* pInstance = new CPlayer(_tInfo);
	//if (!pInstance->Init())
	//	::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Update(void)
{
	if (m_tInfo.m_JGDeth)
		m_tInfo.m_Hp = 100;
}

void CPlayer::move(void)
{

}


void CPlayer::Render(void)
{
}

void CPlayer::Release(void)
{
	delete this;
}

