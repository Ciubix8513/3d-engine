#include "MeshComponent.h"



void Engine::MeshComponent::Shutdown()
{

}
Engine::MeshComponent::MeshComponent()
{
}




ID3D11Buffer** Engine::MeshComponent::GetVertexBufferPtr()
{
	return nullptr;
}

ID3D11Buffer** Engine::MeshComponent::GetIndexBufferPtr()
{
	return nullptr;
}

std::vector<const type_info*> Engine::MeshComponent::GetRequieredComponents()
{
	std::vector<const type_info*> a = { &typeid(Transform) };
	return a;
}

void Engine::MeshComponent::Initialise(std::vector<Component**> c)
{
	transform = (Transform**)&c[0];
	return;
}

bool Engine::MeshComponent::vertex::operator==(vertex& b)
{
	int e = 1;
	vertex* a = this;
	e *= (int)(a->color.x * 10000) == (int)(b.color.x * 10000);
	e *= (int)(a->color.y * 10000) == (int)(b.color.y * 10000);
	e *= (int)(a->color.z * 10000) == (int)(b.color.z * 10000);
	e *= (int)(a->color.w * 10000) == (int)(b.color.w * 10000);
	e *= (int)(a->normal.x * 10000) == (int)(b.normal.x * 10000);
	e *= (int)(a->normal.z * 10000) == (int)(b.normal.z * 10000);
	e *= (int)(a->normal.y * 10000) == (int)(b.normal.y * 10000);
	e *= (int)(a->position.x * 10000) == (int)(b.position.x * 10000);
	e *= (int)(a->position.y * 10000) == (int)(b.position.y * 10000);
	e *= (int)(a->position.z * 10000) == (int)(b.position.z * 10000);
	e *= (int)(a->UV.x * 10000) == (int)(b.UV.x * 10000);
	e *= (int)(a->UV.y * 10000) == (int)(b.UV.y * 10000);
	return e == 1;
}

