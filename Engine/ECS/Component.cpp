#include "Component.h"


Engine::Component::Component()
{
}

/*void Engine::Component::Initialise(D3d* directX)
{
	m_D3d = directX;	
	std::cout << 113;	
	return;
}*/

void Engine::Component::Initialise(std::vector<Component*>,D3d** d3d)
{
	m_D3dPtr = d3d;
}

void Engine::Component::Shutdown()
{
	return;
}

void Engine::Component::Update()
{
}

std::vector<const type_info*> Engine::Component::GetRequieredComponents()
{
	return {};
}
