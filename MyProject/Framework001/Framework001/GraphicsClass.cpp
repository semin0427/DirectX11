////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	CameraClass* m_Camera = 0;
	ModelClass* m_Model = 0;
	ColorShaderClass* m_ColorShader = 0;
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

	// ī�޶� ��ü ����. 
	m_Camera = new CameraClass; 
	if(!m_Camera) 
		return false; 
	
	// ī�޶� �⺻ ��ġ ����. 
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// �� ��ü ����.
	m_Model = new ModelClass;
	if(!m_Model) 
		return false;
	
	// �� ��ü �ʱ�ȭ. 
	result = m_Model->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	// color shader ��ü ����. 
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader)
		return false; 
	
	// color shader ��ü �ʱ�ȭ.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	
	if(!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK); 
		return false; 
	}

	return true;
}


void GraphicsClass::ShutDown()
{
	// color shader ��ü ����.
	if(m_ColorShader)
	{ 
		m_ColorShader->ShutDown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	
	// �� ��ü ����. 
	if(m_Model) 
	{
		m_Model->ShutDown();
		delete m_Model; m_Model = 0; 
	} 
	
	// ī�޶� ��ü ����.
	if(m_Camera)
	{ 
		delete m_Camera;
		m_Camera = 0; 
	}

	//Direct3D��ü ����
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
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	////����� �����ϱ� ���� ���۸� ���.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶� ��ġ�� ����� �� ����� ����.
	m_Camera->Render();

	// ����, ��, ���� ����� ī�޶�, d3d ��ü�κ��� ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix); 
	m_D3D->GetProjectionMatrix(projectionMatrix); 
	
	// �� ������ �ε��� ���۸� �׸������� �׷��� ���������ο� �ִ´�.
	m_Model->Render(m_D3D->GetDeviceContext());
	
	// color ���̴��� �̿��Ͽ� ���� ������ �մϴ�.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result) 
		return false; 

	//�׷��� ����� ȭ�鿡 ǥ��
	m_D3D->EndScene();

	return true;
}