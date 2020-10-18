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
	float yaw, pitch, roll;
	Matrix4x4 rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_PosX;
	position.y = m_PosY;
	position.z = m_PosZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_RotX;
	yaw = m_RotY;
	roll = m_RotZ;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = RotationPitchYawRoll(pitch, yaw , roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt =   (rotationMatrix * Vector4(lookAt, 0)).XYZ();
	up = (rotationMatrix * Vector4(up, 0)).XYZ();

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMat = LookAtMatrix(position, lookAt, up);

	return;
	/*
	float yaw, pitch, roll;
	Matrix4x4 rotMatrix;
	Vector4 lookAtVector = Vector4(0,0,1,0);
	Vector4 upVector = Vector4(0, 1, 0, 0);

	pitch = m_RotX * 0.0174532925f;
	yaw = m_RotY * 0.0174532925f;
	roll = m_RotZ * 0.0174532925f;

	rotMatrix = RotationPitchYawRoll(pitch, yaw, roll);

	
	lookAtVector = rotMatrix * lookAtVector ;
	upVector = rotMatrix * upVector;
	lookAtVector =Vector4( GetPosition() + lookAtVector.XYZ(),0);
	m_viewMat = LookAtMatrix(GetPosition(), lookAtVector.XYZ(), upVector.XYZ());
	return;
	*/
}

Matrix4x4 Camera::GetViewMatrix()
{
	return m_viewMat;
}
