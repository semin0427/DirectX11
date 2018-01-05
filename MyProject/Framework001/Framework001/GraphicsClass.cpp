////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
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

	// 카메라 객체 생성. 
	m_Camera = new CameraClass; 
	if(!m_Camera) 
		return false; 
	
	// 카메라 기본 위치 설정. 
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// 모델 객체 생성.
	m_Model = new ModelClass;
	if(!m_Model) 
		return false;
	
	// 모델 객체 초기화. 
	result = m_Model->Initialize(m_D3D->GetDevice(), L"./Data/Texture/rain_texture.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
		return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::ShutDown()
{
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->ShutDown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	
	// 모델 객체 해제. 
	if(m_Model) 
	{
		m_Model->ShutDown();
		delete m_Model; m_Model = 0; 
	} 
	
	// 카메라 객체 해제.
	if(m_Camera)
	{ 
		delete m_Camera;
		m_Camera = 0; 
	}

	//Direct3D객체 해제
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

	////장면을 시작하기 위해 버퍼를 비움.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라 위치해 기반한 뷰 행렬을 생성.
	m_Camera->Render();
	m_Camera->SetPosition(0, 0, -10.0f);
	// 월드, 뷰, 투영 행렬을 카메라, d3d 객체로부터 얻는다.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix); 
	m_D3D->GetProjectionMatrix(projectionMatrix); 
	
	// 모델 정점과 인덱스 버퍼를 그리기위해 그래픽 파이프라인에 넣는다.
	m_Model->Render(m_D3D->GetDeviceContext());
	
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
		return false;

	//그려진 장면을 화면에 표현
	m_D3D->EndScene();

	return true;
}