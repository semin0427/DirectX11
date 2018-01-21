#pragma once


#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <process.h>
#include <fstream>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "SuperClass.h"
#include "Player.h"
#include "Monster.h"
#include "Controll.h"
#include "Struct.h"




#define KEY_DOWN(vk_code) ((GetKeyState(vk_code) & 0x0000) ? 1 : 0)
