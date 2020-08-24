#pragma once
#pragma region TechStuff
#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma endregion
#pragma region includes
#include <DirectXMath.h>
#pragma endregion

using namespace DirectX;

class Camera
{
#pragma region public funcs
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	void SetPosition(float, float, float);
	void SetPosition(XMFLOAT3);
	void SetRotation(float, float, float);
	void SetRotation(float, float, float,float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);
#pragma endregion
#pragma region private vars
	float m_PosX, m_PosY, m_PosZ,
		  m_RotX, m_RotY, m_RotZ;
	XMMATRIX m_viewMat;
#pragma endregion


};
#endif
