#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include "../Component.h"
#include "../../System/Math/EngineMath.h"

using namespace EngineMath;
namespace Engine 
{

	class Transform : public Component
	{
	public:
		Transform();
		Matrix4x4 GetTransformationMatrix();
		void SetPosition(Vector3 position);
		void SetPosition(float position);
		void SetRotation(Vector3 roation);
		void SetRotation(Quaternion rotation);
		void SetScale(Vector3 scale);
		void SetScale(float scale);

	public:
		Vector3 Position;
		Vector3 Scale;
		Quaternion Rotation;
		int stuff;
	};

}
#endif