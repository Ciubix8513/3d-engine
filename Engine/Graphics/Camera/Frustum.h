#pragma once
#pragma region TechStuff
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_
#pragma endregion
#pragma region includes
#include <DirectXMath.h>
#pragma endregion

using namespace DirectX;

class Frustum
{
#pragma region public funcs
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();
	

	void ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);
	bool CheckPoint(XMFLOAT3 position);
	bool CheckCube(XMFLOAT3 position,float radius);
	bool CheckSphere(XMFLOAT3 position, float radius);
	bool CheckRectangle(float, float, float,float,float,float);
#pragma endregion
#pragma region Public vars
	XMVECTOR  m_Planes[6];
	//bool operator<( XMVECTOR& );	
	friend bool operator <( const XMVECTOR&, float);
	bool operator >=(XMVECTOR&);
	friend bool operator >=(XMVECTOR&, float);
#pragma endregion


};
#endif
