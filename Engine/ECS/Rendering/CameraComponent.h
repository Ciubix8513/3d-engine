#pragma once
#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_
#include "../General/Transform.h"
#include "../Component.h"
using namespace EngineMath;

namespace Engine
{
	class CameraComponent : public Component
	{

		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component**>, D3d** d3d) override;
	public:
		Transform** m_Transform;
	};
};
#endif 

