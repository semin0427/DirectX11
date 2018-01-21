// include the basic windows header files and the Direct3D header file
#include "Defines.h"
// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
//#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DDEVICE9 devFont;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface

// sprite declarations
LPDIRECT3DTEXTURE9 sprite_hero;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite
LPDIRECT3DTEXTURE9 sprite_BattleMap;
LPDIRECT3DTEXTURE9 sprite_Arrow;
LPDIRECT3DTEXTURE9 sprite_MainMap;
LPDIRECT3DTEXTURE9 sprite_Hos;
LPDIRECT3DTEXTURE9 sprite_Over;
LPDIRECT3DTEXTURE9 Cri_EFF;

//font
LPD3DXFONT Font;
ID3DXFont	*font;
std::string message;

									 // function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D

void render_frame(void);    // renders a single frame

///////////////////////////////////
void MainScene();
void render_MainMap();
void render_Player();
void render_Monster();
void render_Text4();
//////////////////////////////////
void BattleScene();
void render_BPlayer();
void render_Arrow();
void render_Text();
void render_Text2();
void render_Text3();
void render_BattleMap();
//////////////////////////////////
void OverScene();
void render_Over();

///////////////////////////////////
void HospitalScene();
//////////////////////////////////.
void render_Cri();
RECT fRectangle;
RECT fRectangle2;
///////////////////////////////////
int Dir;
int moving;
int Player_SIZE = 64;

void cleanD3D(void);		// closes Direct3D and releases memory
void init_gameobject(void); //슈팅 게임 객체 생성 


							// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

using namespace std;


enum { MOVE_DOWN, MOVE_LEFT, MOVE_UP,  MOVE_RIGHT };

CControll con;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";
	

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	// enter the main loop:

	con.MonGen();
	con.Load();

	MSG msg;
	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		render_frame();
		
		switch (con.SceneName)
		{
		case con.MAIN:
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				Dir = MOVE_LEFT;
				moving += 1;
				if (moving > 3)
					moving = 0;

				con.fPposX -= 5;
				con.MapX += 5;
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				moving += 1;
				if (moving > 3)
					moving = 0;

				Dir = MOVE_RIGHT;
				con.fPposX += 5;
				con.MapX -= 5;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				moving += 1;
				if (moving > 3)
					moving = 0;

				Dir = MOVE_UP;
				con.fPposY -= 5;
				con.MapY += 5;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				moving += 1;
				if (moving > 3)
					moving = 0;

				Dir = MOVE_DOWN;
				con.fPposY += 5;
				con.MapY -= 5;
			}
			con.Colision();
			break;
		case con.BATTLE:
			con.fMposX = 400;
			con.fMposY = 100;

			if (GetAsyncKeyState(VK_DOWN) & 0x0001)
			{
				con.Choice();
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x0001)
			{
				con.Select();
			}
			break;
		case con.OVER:	
			if (GetAsyncKeyState(VK_SPACE) & 0x0001)
				PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		case con.HOSPITAL:
			Sleep(1000);
			con.Recover();
			con.Save();
			break;
		}

		// check the 'escape' key
		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"PlayerSprite.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"hero.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"bullet.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"BattleMap.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_BattleMap);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"ArrowUI.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_Arrow);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"MainMap.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_MainMap);    // load to sprite 

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"GameOver.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_Over);

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"HOSPITAL.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_Hos);

	D3DCOLOR ColorKey = D3DCOLOR_XRGB(181, 230, 29);

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"hit.png",    // the file name
		D3DX_DEFAULT_NONPOW2,    // default width
		D3DX_DEFAULT_NONPOW2,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_FILTER_NONE,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		ColorKey,    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&Cri_EFF);		
	
	Font = NULL;

	HRESULT hr = D3DXCreateFont(d3ddev,
		30,
		0,
		FW_NORMAL,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		L"Arial", &Font
	);
	return;
}


// this is the function used to render a single frame
void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency
	
	switch (con.SceneName)
	{
	case 0: 
		MainScene();
		break;
	case 1:
		BattleScene();
		if (con.CriFX)
		{
			render_Cri();
		}
		break;
	case 2:
		OverScene();
		break;
	case con.HOSPITAL:
		HospitalScene();		
		break;
	}

	d3dspt->End();    // end sprite drawing

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}

void MainScene()
{
	render_MainMap();
	render_Player();
	render_Monster();	
}

void BattleScene()
{
	render_BattleMap();
	render_BPlayer();
	render_Arrow();
	render_Monster();
	render_Text();
	render_Text2();
	render_Text3();	
}

void OverScene()
{
	render_Over();
}

//주인공
void render_Player(void)
{	
	//주인공 
	RECT part;
	SetRect(&part, Player_SIZE * Dir, Player_SIZE * moving, Player_SIZE * (Dir+1), Player_SIZE * (moving + 1));
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position(con.fPposX, con.fPposY, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	/*HNum += 1;
	if (HNum > 4)
		HNum = 1;*/
}
//주인공
void render_BPlayer(void)
{
	//주인공 
	RECT part;
	SetRect(&part, 192, 0, 256, 64);
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position(100, 100, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
	/*HNum += 1;
	if (HNum > 4)
	HNum = 1;*/
}
//적
void render_Monster()
{
	RECT part1;
	SetRect(&part1, 0, 0, 64, 64);
	D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position1(con.fMposX, con.fMposY, 0.0f);    // position at 50, 50 with no depth
	d3dspt->Draw(sprite_enemy, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
	/*HNum += 1;
	if (HNum > 4)
		HNum = 1;*/
}
//메인 맵
void render_MainMap()
{
	RECT MMap;
	SetRect(&MMap, 0, 0, 2048, 2048);
	D3DXVECTOR3 MMC(600.0f, 150.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 MMP(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
	D3DXMATRIXA16 matWorld2;
	//D3DXMatrixScaling(&matWorld2, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&matWorld2, con.MapX, con.MapY, 0);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld2);
	d3dspt->Draw(sprite_MainMap, &MMap, &MMC, &MMP, D3DCOLOR_ARGB(255, 255, 255, 255));	
}
//배틀맵
void render_BattleMap()
{
	RECT BMap;
	SetRect(&BMap, 0, 0, 586, 269);
	D3DXVECTOR3 BMC(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 BMP(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
	D3DXMATRIXA16 matWorld2;
	D3DXMatrixScaling(&matWorld2, 1.1f, 1.1f, 1.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld2);
	d3dspt->Draw(sprite_BattleMap, &BMap, &BMC, &BMP, D3DCOLOR_ARGB(255, 255, 255, 255));
}
//병원
void HospitalScene()
{
	RECT HOS;
	SetRect(&HOS, -20, -20, 344, 146);
	D3DXVECTOR3 HMC(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 HMP(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
	D3DXMATRIXA16 matWorld2;
	D3DXMatrixScaling(&matWorld2, 2.0f, 2.0f, 1.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld2);
	d3dspt->Draw(sprite_Hos, &HOS, &HMC, &HMP, D3DCOLOR_ARGB(255, 255, 255, 255));
}
//게임오버
void render_Over()
{
	RECT Over;
	SetRect(&Over, 0, 0, 599, 344);
	D3DXVECTOR3 OMC(0, 0, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 OMP(0.0f, 0.0f, 0.0f);    // position at 50, 50 with no depth
	D3DXMATRIXA16 matWorld2;
	D3DXMatrixScaling(&matWorld2, 1.1f, 1.4f, 1.0f);
	//D3DXMatrixTranslation(&matWorld2, 0, 0, 0);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld2);
	d3dspt->Draw(sprite_Over, &Over, &OMC, &OMP, D3DCOLOR_ARGB(255, 255, 255, 255));
}
//화살표
void render_Arrow()
{
	////화살표 
	RECT part2;
	SetRect(&part2, 0, 0, 32, 25);
	D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 position2(400.0f, con.ypos, 0.0f);    // position at 50, 50 with no depth
	/*D3DXMATRIXA16 matWorld;
	D3DXMatrixScaling(&matWorld, 0.3f, 0.3f, 1.0f);   
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld);*/
	d3dspt->Draw(sprite_Arrow, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
}
//주인공 스텟
void render_Text()
{
	char string[100];
	SetRect(&fRectangle, 50, 300, 1000, 1000);
	sprintf(string, " <나 : %d Lv>\n 체력 : %d\n 공격 : %d\n 방어 : %d\n 경험치 : %d\n", con.LV, con.mPlayer->GetINFO().m_Hp, con.mPlayer->GetINFO().m_Att, con.mPlayer->GetINFO().m_Def, con.mPlayer->GetINFO().m_Spd);
	Font->DrawTextA(NULL, string, -1, &fRectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
//적 스텟
void render_Text2()
{
	char string[100];
	SetRect(&fRectangle, 300, 300, 1000, 1000);
	sprintf(string, "  <몬스터>\n체력 : %d\n공격 : %d\n방어 : %d\n경험치 : %d\n", con.mMonster->GetINFO().m_Hp, con.mMonster->GetINFO().m_Att, con.mMonster->GetINFO().m_Def, con.mMonster->GetINFO().m_Spd);
	Font->DrawTextA(NULL, string, -1, &fRectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
//메뉴
void render_Text3()
{
	char string[100];
	SetRect(&fRectangle, 500, 305, 1000, 1000);
	sprintf(string, " \n 싸운다 \n\n 도망간다");
	Font->DrawTextA(NULL, string, -1, &fRectangle, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}

void render_Cri()
{
	RECT Cri;
	SetRect(&Cri, 0, 0, 66, 75);
	D3DXVECTOR3 CMC(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	D3DXVECTOR3 CMP(420.0f, 70.0f, 0.0f);    // position at 50, 50 with no depth
	D3DXMATRIXA16 matWorld2;
	/*D3DXMatrixScaling(&matWorld2, 2.0f, 2.0f, 1.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &matWorld2);*/
	d3dspt->Draw(Cri_EFF, &Cri, &CMC, &CMP, D3DCOLOR_ARGB(255, 255, 255, 255));
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();	
	sprite_BattleMap->Release();
	sprite_MainMap->Release();

	return;
}