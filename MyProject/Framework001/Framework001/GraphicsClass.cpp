////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	//m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
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
	result = m_Model->Initialize(m_D3D->GetDevice(), "./Data/Model/Cube.txt", L"./Data/Texture/rain_texture.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	//// Create the texture shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//	return false;

	//// Initialize the texture shader object.
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}


void GraphicsClass::ShutDown()
{
	// Release the texture shader object.
	/*if (m_TextureShader)
	{
		m_TextureShader->ShutDown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}*/
	
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->ShutDown();
		delete m_LightShader;
		m_LightShader = 0;
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
	static float rotation = 0.0f;
	float rotationSpd = 1.00f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f * rotationSpd;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	bool result;

	result = Render(rotation);
	
	if (!result)
		return false;

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	////����� �����ϱ� ���� ���۸� ���.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ī�޶� ��ġ�� ����� �� ����� ����.
	m_Camera->Render();
	//m_Camera->SetRotation(10, 0, 0);
	//m_Camera->SetPosition(1, 1, -15.0f);
	// ����, ��, ���� ����� ī�޶�, d3d ��ü�κ��� ��´�.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix); 
	m_D3D->GetProjectionMatrix(projectionMatrix); 
	
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	// �� ������ �ε��� ���۸� �׸������� �׷��� ���������ο� �ִ´�.
	m_Model->Render(m_D3D->GetDeviceContext());
	
	// Render the model using the texture shader.
	/*result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
		return false;*/

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return false;
	}

	//�׷��� ����� ȭ�鿡 ǥ��
	m_D3D->EndScene();

	return true;
}