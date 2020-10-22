#include "Camera.h"

Camera::Camera()
{
	m_PosX = 0.0f;
	m_PosY = 0.0f;
	m_PosZ = 0.0f;
	
	m_RotX = 0.0f;
	m_RotY = 0.0f;
	m_RotZ = 0.0f;
}

Camera::Camera(const Camera&)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

void Camera::SetPosition(Vector3 pos)
{
	m_PosX =pos.x;
	m_PosY = pos.y;
	m_PosZ = pos.z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_RotX = x;
	m_RotY = y;
	m_RotZ = z;
}

void Camera::SetRotation(float x, float y, float z, float w)
{
	//TODO: impliment quaternions
}

Vector3 Camera::GetPosition()
{
	return Vector3(m_PosX,m_PosY,m_PosZ);
}

Vector3 Camera::GetRotation()
{
	return Vector3(m_RotX, m_RotY, m_RotZ);
}

void Camera::Render()
{
	Vector3 up, position, lookAt;	
	Matrix4x4 rotationMatrix;	

	up.y = 1.0f;
	lookAt.z = 1.0f;
	position = GetPosition();	

	rotationMatrix = RotationPitchYawRoll(GetRotation());
	lookAt =   (rotationMatrix * Vector4(lookAt, 0)).XYZ();
	up = (rotationMatrix * Vector4(up, 0)).XYZ();	
	lookAt = position + lookAt;

	m_viewMat = LookAtMatrix(position, lookAt, up);
	return;
	
}

Matrix4x4 Camera::GetViewMatrix()
{
	return m_viewMat;
}
