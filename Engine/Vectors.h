#pragma once
namespace Engine
{

	struct Vector3
	{
	public:
		float x, y, z;
		
	};
	struct Vector4
	{
	public:
		float x, y, z, w;
		Vector4();
		Vector4(float, float, float, float);
	};
}