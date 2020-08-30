#include "Frustum.h"

Frustum::Frustum()
{
}
Frustum::Frustum(const Frustum&)
{
}
Frustum::~Frustum()
{
}


void Frustum::ConstructFrustum(float screenDepth, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	float minimumZ, r;
	XMMATRIX matrix;
	XMFLOAT4 proj[4],frustum[4],planes[6];
	
	for (int i = 0; i < 4; i++)	
		XMStoreFloat4(&proj[i], projectionMatrix.r[i]);
	
	minimumZ = -proj[3].z / proj[2].y;
	r = screenDepth / (screenDepth - minimumZ);
	projectionMatrix = XMMatrixSet
	   (proj[0].x, proj[0].y, proj[0].z, proj[0].w,
		   proj[1].x, proj[1].y, proj[1].z, proj[1].w,
		   proj[2].x, proj[2].y, r, proj[2].w,
		   proj[3].x, proj[3].y, -r*minimumZ, proj[3].w);
	matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
	for (int i = 0; i < 4; i++)
		XMStoreFloat4(&frustum[i], matrix.r[i]);

	//near plane
	planes[0].x = frustum[0].w + frustum[0].z;
	planes[0].y = frustum[1].w + frustum[1].z;
	planes[0].z = frustum[2].w + frustum[2].z;
	planes[0].w = frustum[3].w + frustum[3].z;
	
	//far plane
	planes[1].x = frustum[0].w - frustum[0].z;
	planes[1].y = frustum[1].w - frustum[1].z;
	planes[1].z = frustum[2].w - frustum[2].z;
	planes[1].w = frustum[3].w - frustum[3].z;

	//left plane
	planes[2].x = frustum[0].w + frustum[0].x;
	planes[2].y = frustum[1].w + frustum[1].x;
	planes[2].z = frustum[2].w + frustum[2].x;
	planes[2].w = frustum[3].w + frustum[3].x;

	//right plane
	planes[3].x = frustum[0].w - frustum[0].x;
	planes[3].y = frustum[1].w - frustum[1].x;
	planes[3].z = frustum[2].w - frustum[2].x;
	planes[3].w = frustum[3].w - frustum[3].x;

	//top plane
	planes[4].x = frustum[0].w - frustum[0].y;
	planes[4].y = frustum[1].w - frustum[1].y;
	planes[4].z = frustum[2].w - frustum[2].y;
	planes[4].w = frustum[3].w - frustum[3].y;

	//bottom plane
	planes[5].x = frustum[0].w + frustum[0].y;
	planes[5].y = frustum[1].w + frustum[1].y;
	planes[5].z = frustum[2].w + frustum[2].y;
	planes[5].w = frustum[3].w + frustum[3].y;

	for (int i = 0; i < 6; i++)
	{
		m_Planes[i] = XMLoadFloat4(&planes[i]);
		m_Planes[i] = XMPlaneNormalize(m_Planes[i]);
	}

	return;
	
}

bool Frustum::CheckPoint(XMFLOAT3 position)
{
	for (int i = 0; i < 6; i++)	
		if (XMPlaneDotCoord(m_Planes[i], XMLoadFloat3(&position)) < 0.0f)
			return false;
	return true;
}

bool Frustum::CheckCube(XMFLOAT3 position, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (XMPlaneDotCoord(m_Planes[i], XMLoadFloat3(&XMFLOAT3()) >= 0.0f))
		{
			continue; 
		}
	}

	return true;
}

bool Frustum::CheckSphere(XMFLOAT3 position, float radius)
{
	return true;
}

bool Frustum::CheckRectangle(float, float, float, float, float, float)
{
	return true;
}


bool operator<(XMVECTOR& v, float f)
{
	for (int i = 0; i < 4; i++)
		if (v.m128_f32[i] > f)
			return false;
	return true;
}

bool operator>=(XMVECTOR& v, float f)
{
	for (int i = 0; i < 4; i++)
		if (v.m128_f32[i] < f)
			return false;
	return true;
}
