#include "ModelClass.h"



ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if (!result)
		return false;

	//삼각형의 기하 정보를 가지고 있는 버텍스, 인덱스 버퍼를 초기화
	result = InitializeBuffers(device);
	if (!result)
		return false;

	// 현재 모델을 위한 텍스쳐 로딩
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;
	
	return true;
}

void ModelClass::ShutDown()
{
	// 텍스쳐 해제
	ReleaseTexture();

	//버텍스 인덱스버퍼 해제
	ShutDownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	//버텍스 인덱스 버퍼를 그려질 수 있도록 그래픽 파이프라인에 넣음
	RenderBuffers(deviceContext);

	return;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	////버텍스 배열의 정점갯수 설정.
	//m_vertexCount = 6;

	////인덱스 배열의 인덱스 수 설정.
	//m_indexCount = 6;

	//버텍스 배열생성.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//인덱스 배열 생성.
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// 버텍스 배열에 데이터를 넣음. 
	/*
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // 왼쪽 아래 
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); 	// 왼쪽 위
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); 	// 오른쪽 위
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[3].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // 왼쪽 아래 
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); 	// 오른쪽 위
	vertices[4].texture = D3DXVECTOR2(0.5f, 1.0f);
	vertices[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[5].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // 오른쪽 아래 
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	
	// 인덱스 배열에 데이터를 넣음. 
	indices[0] = 0; // 왼쪽 아래
	indices[1] = 1; // 왼쪽 위 
	indices[2] = 2; // 오른쪽 위 
	indices[3] = 3; // 왼쪽 아래
	indices[4] = 4; // 오른쪽 위 
	indices[5] = 5; // 오른쪽 아래
	*/


	// 정적 정점 버퍼의 description을 작성.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	// 버텍스 데이터(버텍스 배열)의 포인터를 subresource 구조체에 넣음. 
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	// 버텍스 버퍼를 생성.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;
	
	// 정적 인덱스 버퍼의 description을 작성. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// 인덱스 데이터의 포인터를 subresource 구조체에 넣음. 
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// 인덱스 버퍼 생성.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// 버텍스, 인덱스 버퍼가 생성되었으므로 배열을 해제. 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutDownBuffers()
{
	//인덱스 버퍼 해제.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//버텍스 버퍼 해제.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// 버텍스 버퍼 걸음(단위)와 오프셋을 설정. 
	stride = sizeof(VertexType); 
	offset = 0; 
	
	// 어셈블러 입력에 버텍스 버퍼를 활성화 시켜 렌더링 될 수 있음. 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset); 
	
	// 어셈블러 입력에 인덱스 버퍼를 활성화 시켜 렌더링 될 수 있음. 
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0); 
	
	// 이 버텍스 버퍼로부터 그릴 기본 자료형을 설정. 이 경우 삼각형.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// 텍스쳐 객체 생성
	m_Texture = new TextureClass;
	if (!m_Texture)
		return false;

	// 텍스쳐 객체 초기화
	result = m_Texture->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	// 텍스쳐 객체 해제
	if (m_Texture)
	{
		m_Texture->ShutDown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
		return false;

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}