////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Direct3D 객체 생성.
	m_D3D = new D3DClass;
	if (!m_D3D)
		return false;

	//Direct3D 객체 초기화
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->ShutDown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	
	if (!result)
		return false;

	return true;
}


bool GraphicsClass::Render()
{
	////장면을 시작하기 위해 버퍼를 비움.
	m_D3D->BeginScene(0.0f, 0.5f, 0.5f, 1.0f);

	////그려진 장면을 화면에 표현
	m_D3D->EndScene();

	return true;
}