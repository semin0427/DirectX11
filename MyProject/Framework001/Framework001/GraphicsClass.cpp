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

	//Direct3D ��ü ����.
	m_D3D = new D3DClass;
	if (!m_D3D)
		return false;

	//Direct3D ��ü �ʱ�ȭ
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
	////����� �����ϱ� ���� ���۸� ���.
	m_D3D->BeginScene(0.0f, 0.5f, 0.5f, 1.0f);

	////�׷��� ����� ȭ�鿡 ǥ��
	m_D3D->EndScene();

	return true;
}