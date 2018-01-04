#include "CameraClass.h"



CameraClass::CameraClass()
{
	m_positionX, m_positionY, m_positionZ = 0.0f;
	m_rotationX, m_rotationY, m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;
	
	// up 벡터 설정.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// 월드좌표에서 카메라 위치를 설정.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// 카메라가 보는 곳을 기본으로 설정.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// yaw (Y축), pitch (X축), roll (Z축) 회전 값을 라디안값으로 설정.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// yaw, pitch, roll 값으로 회전 행렬을 만든다.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// lookAt, up 벡터를 회전 행렬로 회전 변환. 이제 뷰가 원점에서 회전된다.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// 회전된 카메라 위치가 적용된 lookAt을 얻는다.
	lookAt = position + lookAt;

	// 마지막으로 세개의 설정된 벡터로 뷰 행렬을 생성.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}