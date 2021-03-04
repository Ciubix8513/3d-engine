#include "MaterialComponent.h"

std::vector<const type_info*> Engine::MaterialComponent::GetRequieredComponents()
{
	return {&typeid(Engine::MeshComponent)};
}

size_t Engine::MaterialComponent::GetRenderingOrder()
{
	return RenderingOrder;
}

void Engine::MaterialComponent::SetRenderingOrder(size_t NewRenderingOrder)
{
	RenderingOrder = NewRenderingOrder;
	return;
}
