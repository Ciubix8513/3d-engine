#pragma once
#include "MeshComponent.h"
using namespace Engine;
using namespace EngineMath;

namespace Engine 
{
	class MaterialComponent : public Component
	{
		std::vector<const type_info*> GetRequieredComponents() override;
	};
}
