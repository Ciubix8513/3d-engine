#pragma once
#pragma region TechStuff
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_
#pragma endregion
#include "..//../System/Math/EngineMath.h"
using namespace EngineMath;

class Frustum
{
#pragma region public funcs
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();
	

	void ConstructFrustum(float screenDepth, Matrix4x4 projectionMatrix, Matrix4x4 viewMatrix);
	bool CheckPoint(Vector3 position);
	bool CheckCube(Vector3 position,float radius);
	bool CheckSphere(Vector3 position, float radius);
	bool CheckRectangle(float, float, float,float,float,float);
#pragma endregion
#pragma region Public vars
	Plane  m_Planes[6];


#pragma endregion


};
#endif
