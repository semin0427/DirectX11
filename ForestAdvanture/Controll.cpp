#include "Controll.h"

CControll::CControll()
{
	ALL_INFO _tInfo(1, 1, 1, 1);
	_tInfo = Info(Patt, Pdef, Pexp, Php);
	mPlayer = CPlayer::Create(_tInfo);
	int mon;
	mon = rand() % 3 + 1;

	if (mon == 1)
	{
		_tInfo = Info(13, 5, 5, 100);
	}
	else if(mon == 2)
		_tInfo = Info(11, 8, 5, 100);
	else if(mon == 3)
		_tInfo = Info(15, 0, 5, 100);

	mMonster = CMonster::Create(_tInfo);
}


CControll::~CControll()
{
}

void CControll::Select()
{

	HP = mPlayer->GetINFO().m_Hp;
	PDmg = mPlayer->GetINFO().m_Att - mMonster->GetINFO().m_Def;
	MDmg = mMonster->GetINFO().m_Att - mPlayer->GetINFO().m_Def;

	if (fight == true && run == false)
	{
		if (Cri == rand() % 3 + 1)
		{
			HP = mMonster->GetINFO().m_Hp - (PDmg * 2);
			CriFX = true;
		}
		else
		{
			HP = mMonster->GetINFO().m_Hp - PDmg;
			CriFX = false;
		}

		ALL_INFO CurrentPState = Info(att, def, exp, HP);
		mMonster->SetINFO(CurrentPState);
		
		if (HP <= 0)
		{
			Pexp += mMonster->GetINFO().m_Spd;
			SceneName = MAIN;
			HP = 100;
			MonGen();
		}

		HP = mPlayer->GetINFO().m_Hp - MDmg;
		ALL_INFO CurrentMState = Info(Patt, Pdef, Pexp, HP);
		mPlayer->SetINFO(CurrentMState);

		if (HP <= 0)
		{
			SceneName = OVER;
		}
	}
	else if (fight == false && run == true)
	{
		SceneName = MAIN;		
		MonGen();
	}

	if (Pexp >= 100)
	{
		LV += 1;
		Pexp = 0;
		Patt += Uatt;
		Pdef += Udef;
		Php += Uhp;
		HP = Php;
		ALL_INFO CurrentMState = Info(Patt, Pdef, Pexp, HP);
		mPlayer->SetINFO(CurrentMState);
	}

}

void CControll::Choice()
{
	if (ypos < y)
	{
		ypos += 50;

		fight = false;
		run = true;		
	}
	else if (ypos >= y)
	{
		ypos -= 50;
		
		fight = true;
		run = false; 
	}
}

void CControll::Colision()
{
	if(fPposX + HSize >= fMposX
		&& fPposX <= fMposX + HSize
		&& fPposY + HSize >= fMposY
		&& fPposY <= fMposY + HSize)
	{
		SceneName = BATTLE;		
	}
	else if (fPposX + HSize >= -10
		&& fPposX <= 0
		&& fPposY + HSize >= 1220
		&& fPposY <= 1240
		&& SceneName == MAIN)
	{
		SceneName = HOSPITAL;
	}
	else
		SceneName = MAIN;
}

void CControll::MonGen()
{
	ALL_INFO _tInfo(1, 1, 1, 1);


	RANNUM = rand() % 3 + 1;
	if (temp == RANNUM)
	{
		RANNUM = rand() % 3 + 1;
	}
	temp = RANNUM;

	switch (RANNUM)
	{
	case 1:
		fMposX = 400;
		fMposY = 100;
		att = 13;
		def = 5;
		exp = 10;
		hp = 100;
		_tInfo = Info(att, def, exp, hp);
		mMonster = CMonster::Create(_tInfo);
		break;
	case 2:
		fMposX = 200;
		fMposY = 500;
		att = 15;
		def = 0;
		exp = 50;
		hp = 100;
		_tInfo = Info(att, def, exp, hp);
		mMonster = CMonster::Create(_tInfo);
		break;
	case 3:
		fMposX = 35;
		fMposY = 300;
		att = 11;
		def = 5;
		exp = 90;
		hp = 120;
		_tInfo = Info(att, def, exp, hp);
		mMonster = CMonster::Create(_tInfo);
	}
}

void CControll::Recover()
{
	SceneName = MAIN;
	
	fPposX = -5;
	fPposY = 1250;
	
	MapX = 300;
	MapY = -1024;

	HP = Php;
	ALL_INFO CurrentMState = Info(Patt, Pdef, Pexp, HP);
	mPlayer->SetINFO(CurrentMState);
}

void CControll::Save()
{
	ofstream SaveFile("Save.txt");
	SaveFile << mPlayer->GetINFO().m_Att << endl;
	SaveFile << mPlayer->GetINFO().m_Def << endl;
	SaveFile << mPlayer->GetINFO().m_Hp << endl;
	SaveFile << mPlayer->GetINFO().m_Spd << endl;
	SaveFile << LV << flush;

	SaveFile.close();
}

void CControll::Load()
{
	ifstream LoadFile("Save.txt");
	
	char Att[32];
	char Def[32];
	char HP[32];
	char EXP[32];
	char Lv[32];

	while (!LoadFile.eof())
	{		
		LoadFile.getline(Att, 32);
		LoadFile.getline(Def, 32);
		LoadFile.getline(HP, 32);
		LoadFile.getline(EXP, 32);
		LoadFile.getline(Lv, 32);

		Php = atoi(HP);
		Patt = atoi(Att);
		Pdef = atoi(Def);
		Pexp = atoi(EXP);
		LV = atoi(Lv);
	}

	ALL_INFO CurrentMState = Info(Patt, Pdef, Pexp, Php);
	mPlayer->SetINFO(CurrentMState);
}
