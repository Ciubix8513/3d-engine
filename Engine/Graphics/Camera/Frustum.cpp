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
	XMFLOAT4 proj[4],frustum[4];
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
}

bool Frustum::CheckPoint(XMFLOAT3 position)
{
	return false;
}

bool Frustum::CheckCube(XMFLOAT3 position, float radius)
{
	return false;
}

bool Frustum::CheckSphere(XMFLOAT3 position, float radius)
{
	return false;
}

bool Frustum::CheckRectangle(float, float, float, float, float, float)
{
	return false;
}
