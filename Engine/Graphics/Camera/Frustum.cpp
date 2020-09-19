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


void Frustum::ConstructFrustum(float screenDepth, Matrix4x4 projectionMatrix, Matrix4x4 viewMatrix)
{
	float minimumZ, r;
	Matrix4x4 frustum;
	minimumZ = -projectionMatrix._m32 / projectionMatrix._m22;
	r = screenDepth / (screenDepth - minimumZ);
	projectionMatrix._m22 = r;
	projectionMatrix._m32 = -r * minimumZ;

	frustum = viewMatrix * projectionMatrix;

	//near plane
	m_Planes[0].a = frustum[0].w + frustum[0].z;
	m_Planes[0].b = frustum[1].w + frustum[1].z;
	m_Planes[0].c = frustum[2].w + frustum[2].z;
	m_Planes[0].d = frustum[3].w + frustum[3].z;
	
	//far plane
	m_Planes[1].a = frustum[0].w - frustum[0].z;
	m_Planes[1].b = frustum[1].w - frustum[1].z;
	m_Planes[1].c = frustum[2].w - frustum[2].z;
	m_Planes[1].d = frustum[3].w - frustum[3].z;

	//left plane
	m_Planes[2].a = frustum[0].w + frustum[0].x;
	m_Planes[2].b = frustum[1].w + frustum[1].x;
	m_Planes[2].c = frustum[2].w + frustum[2].x;
	m_Planes[2].d = frustum[3].w + frustum[3].x;

	//right plane
	m_Planes[3].a = frustum[0].w - frustum[0].x;
	m_Planes[3].b = frustum[1].w - frustum[1].x;
	m_Planes[3].c = frustum[2].w - frustum[2].x;
	m_Planes[3].d = frustum[3].w - frustum[3].x;

	//top plane
	m_Planes[4].a = frustum[0].w - frustum[0].y;
	m_Planes[4].b = frustum[1].w - frustum[1].y;
	m_Planes[4].c = frustum[2].w - frustum[2].y;
	m_Planes[4].d = frustum[3].w - frustum[3].y;

	//bottom plane
	m_Planes[5].a = frustum[0].w + frustum[0].y;
	m_Planes[5].b = frustum[1].w + frustum[1].y;
	m_Planes[5].c = frustum[2].w + frustum[2].y;
	m_Planes[5].d = frustum[3].w + frustum[3].y;

	return;
	
}

bool Frustum::CheckPoint(Vector3 position)
{
	for (int i = 0; i < 6; i++)	
		if (PlaneDotCoord(m_Planes[i],position) < 0.0f)
			return false;
	return true;
}

bool Frustum::CheckCube(Vector3 position, float radius)
{

	for (int i = 0; i < 6; i++)
	{
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x - radius), (position.y - radius), (position.z - radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x + radius), (position.y - radius), (position.z - radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x - radius), (position.y + radius), (position.z - radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x + radius), (position.y + radius), (position.z - radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x - radius), (position.y - radius), (position.z + radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x + radius), (position.y - radius), (position.z + radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x - radius), (position.y + radius), (position.z + radius))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((position.x + radius), (position.y + radius), (position.z + radius))) >= 0.0f)
			continue;
		return false;
	}

	return true;
}

bool Frustum::CheckSphere(Vector3 position, float radius)
{
	for (int i = 0; i < 6; i++)	
		if (PlaneDotCoord(m_Planes[i], position) < -radius)
			return false;

	return true;
}

bool Frustum::CheckRectangle(float PositionX, float PositionY, float PositionZ, float SizeX, float SizeY, float SizeZ)
{
	for ( int i = 0; i < 6;  i++)
	{
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX - SizeX), (PositionY - SizeY), (PositionZ - SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX + SizeX), (PositionY - SizeY), (PositionZ - SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX - SizeX), (PositionY + SizeY), (PositionZ - SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX - SizeX), (PositionY - SizeY), (PositionZ + SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX + SizeX), (PositionY + SizeY), (PositionZ - SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX + SizeX), (PositionY - SizeY), (PositionZ + SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX - SizeX), (PositionY + SizeY), (PositionZ + SizeZ))) >= 0.0f)
			continue;
		if (PlaneDotCoord(m_Planes[i], Vector3((PositionX + SizeX), (PositionY + SizeY), (PositionZ + SizeZ))) >= 0.0f)
			continue;
		return false;
	}
	return true;
}



