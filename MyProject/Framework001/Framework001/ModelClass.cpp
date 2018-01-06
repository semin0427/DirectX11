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

	//�ﰢ���� ���� ������ ������ �ִ� ���ؽ�, �ε��� ���۸� �ʱ�ȭ
	result = InitializeBuffers(device);
	if (!result)
		return false;

	// ���� ���� ���� �ؽ��� �ε�
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;
	
	return true;
}

void ModelClass::ShutDown()
{
	// �ؽ��� ����
	ReleaseTexture();

	//���ؽ� �ε������� ����
	ShutDownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	//���ؽ� �ε��� ���۸� �׷��� �� �ֵ��� �׷��� ���������ο� ����
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

	////���ؽ� �迭�� �������� ����.
	//m_vertexCount = 6;

	////�ε��� �迭�� �ε��� �� ����.
	//m_indexCount = 6;

	//���ؽ� �迭����.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//�ε��� �迭 ����.
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

	// ���ؽ� �迭�� �����͸� ����. 
	/*
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // ���� �Ʒ� 
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); 	// ���� ��
	vertices[1].texture = D3DXVECTOR2(0.5f, 0.0f);
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); 	// ������ ��
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[3].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // ���� �Ʒ� 
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); 	// ������ ��
	vertices[4].texture = D3DXVECTOR2(0.5f, 1.0f);
	vertices[4].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertices[5].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // ������ �Ʒ� 
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);
	vertices[5].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	
	// �ε��� �迭�� �����͸� ����. 
	indices[0] = 0; // ���� �Ʒ�
	indices[1] = 1; // ���� �� 
	indices[2] = 2; // ������ �� 
	indices[3] = 3; // ���� �Ʒ�
	indices[4] = 4; // ������ �� 
	indices[5] = 5; // ������ �Ʒ�
	*/


	// ���� ���� ������ description�� �ۼ�.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	// ���ؽ� ������(���ؽ� �迭)�� �����͸� subresource ����ü�� ����. 
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	// ���ؽ� ���۸� ����.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
		return false;
	
	// ���� �ε��� ������ description�� �ۼ�. 
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// �ε��� �������� �����͸� subresource ����ü�� ����. 
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	
	// �ε��� ���� ����.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
		return false;

	// ���ؽ�, �ε��� ���۰� �����Ǿ����Ƿ� �迭�� ����. 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::ShutDownBuffers()
{
	//�ε��� ���� ����.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//���ؽ� ���� ����.
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

	// ���ؽ� ���� ����(����)�� �������� ����. 
	stride = sizeof(VertexType); 
	offset = 0; 
	
	// ����� �Է¿� ���ؽ� ���۸� Ȱ��ȭ ���� ������ �� �� ����. 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset); 
	
	// ����� �Է¿� �ε��� ���۸� Ȱ��ȭ ���� ������ �� �� ����. 
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0); 
	
	// �� ���ؽ� ���۷κ��� �׸� �⺻ �ڷ����� ����. �� ��� �ﰢ��.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// �ؽ��� ��ü ����
	m_Texture = new TextureClass;
	if (!m_Texture)
		return false;

	// �ؽ��� ��ü �ʱ�ȭ
	result = m_Texture->Initialize(device, filename);
	if (!result)
		return false;

	return true;
}

void ModelClass::ReleaseTexture()
{
	// �ؽ��� ��ü ����
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