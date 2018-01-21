#pragma once
#include <d3d9.h>

typedef struct Info
{
	int m_Att;
	int m_Def;	
	int m_Spd;
	int m_Hp;
	float m_xPos;
	float m_yPos;
	bool m_JGDeth;
	Info(int _Att, int _Def, int _Spd, int _Hp)
		:m_Att(_Att),
		m_Def(_Def),		
		m_Spd(_Spd),
		m_Hp(_Hp),		
		m_JGDeth(false)
	{}
}ALL_INFO;