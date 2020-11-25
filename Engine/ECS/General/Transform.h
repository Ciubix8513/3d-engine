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
		 void Shutdown() override;

		Matrix4x4 GetTransformationMatrix();
#pragma region  SetProperties
		void SetChild(Transform** Child);
		void SetParrent(Transform** Parent);
		void SetPosition(Vector3 position);
		void SetPosition(float position);
		void SetRotation(Vector3 roation);
		void SetRotation(Quaternion rotation);
		void SetScale(Vector3 scale);
		void SetScale(float scale);
#pragma endregion
		Vector3 GetReletivePosition();
		Vector3 GetAbsolutePosition();
		Vector3 GetReletiveScale();
		Vector3 GetAbsoluteScale();
		Quaternion GetReletiveRotation();
		Quaternion GetAbsoluteRotation();
		Transform** GetParent();
		Transform** GetChild(unsigned int ChildIndex);
		Transform*** GetChildren(unsigned int ChildIndex0, unsigned int ChildIndex1);
		Transform*** GetChildren(unsigned int* ChildrenIndecies, unsigned int ChildrenCount);
		Transform*** GetChildren(vector<unsigned int> ChildrenIndecies);
		bool HasParent();
		bool HasChildren();
		unsigned int GetChildrenCount();
		void RemoveParent();
		void RemoveChild(Transform** Child);
		void RemoveChildren(Transform*** Children, unsigned int ChildrenCount);
		void RemoveChildren(unsigned int* ChildrenIndecies, unsigned int ChildrenCount);
		void RemoveChildren(unsigned int ChildIndex0, unsigned int ChildIndex1);
		void RemoveAllChildren();
	public:
		Vector3 Position;
		Vector3 Scale;
		Quaternion Rotation;
		int stuff;

	private:
		vector<Transform**> m_Children;
		Transform** m_Parent;
		bool HasPar;
		
	};

}
#endif