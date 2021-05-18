#pragma once
#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_
#include "../General/Transform.h"
#include "../Component.h"
using namespace EngineMath;

#undef near
#undef far
namespace Engine
{
	class CameraComponent : public Component
	{
	public:
		enum CameraType
		{
			Perspective, Ortho
		};
	private:
		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component*>, D3d** d3d, ULONG entityUUID) override;
		std::string GetName() override;

		void Update() override;
	public:
		Matrix4x4 GetProjectionMatrix();
		Matrix4x4 GetViewMatrix();
		
		void SetCameraParams(CameraType type, float fov, float near, float far,int screenWidth, int screenHeight);
	private: 
		Matrix4x4 m_projMat;
		Matrix4x4 m_viewMat;
		CameraType m_Type;
		float m_FOV;
		float m_ScreenAspect;
		 float m_ScreenNear;
		float m_ScreenFar;
		int m_screenWidth;
		int m_screenHeight;
	public:
		Transform* m_Transform;
	};
};
#endif 

