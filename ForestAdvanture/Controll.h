#pragma once
#include "Defines.h"

class CSuperClass;
class CControll
{
public:
	CControll();
	~CControll();

public:
	void Select();
	void Choice();
	void Colision();
	void MonGen();
	void Recover();
	void LvUp();
	void Save();
	void Load();

public:
	float ypos = 300;
	float y = 350;
	bool fight = true;
	bool run = false;	
	bool CriFX = false;

	CSuperClass* mPlayer;
	CSuperClass* mMonster;

	int SceneName = 0;

	int PDmg;
	int MDmg;
	int Cri = 3;
	
	int HP;

	float fPposX = 270;
	float fPposY = 190;

	float fMposX = 400;
	float fMposY = 100;

	float MapX = 0;
	float MapY = 0;

	int HSize = 50;
	int HNum = 0;
	
	int temp = 0;
	int RANNUM;	

	int att;
	int def;
	int exp;
	int hp;

	int Patt = 10;
	int Pdef = 5;
	int Pexp = 0;
	int Php = 150;

	int LV = 1;
	int Uatt = 5;
	int Udef = 1;
	int Uexp = 0;
	int Uhp = 10;

	HWND hWnd;;


public:
	enum { MAIN = 0, BATTLE =1, OVER = 2, HOSPITAL = 3};
	enum { Mon1 = 1, Mon2 = 2, Mon3 = 3};
};

