#include "CameraComponent.h"

std::vector<const type_info*> Engine::CameraComponent::GetRequieredComponents()
{
	return {&typeid(Transform)};
}

void Engine::CameraComponent::Initialise(std::vector<Component**> Components)
{
	m_Transform = (Transform**)Components[0];
	return;
}