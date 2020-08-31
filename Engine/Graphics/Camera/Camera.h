#pragma once
#pragma region TechStuff
#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma endregion
#pragma region includes
#include "../../System/Math/EngineMath.h"
#pragma endregion

using namespace EngineMath;

class Camera
{
#pragma region public funcs
public:
	Camera();
	Camera(const Camera&);
	~Camera();
	void SetPosition(float, float, float);
	void SetPosition(Vector3);
	void SetRotation(float, float, float);
	void SetRotation(float, float, float,float);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void Render();
	void GetViewMatrix(Matrix4x4&);
#pragma endregion
#pragma region private vars
	float m_PosX, m_PosY, m_PosZ,
		  m_RotX, m_RotY, m_RotZ;
	Matrix4x4 m_viewMat;
#pragma endregion


};
#endif
