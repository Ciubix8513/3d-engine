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

void Engine::Component::Initialise(std::vector<Component*>,D3d** d3d, ULONG entityUUID)
{
	m_D3dPtr = d3d;
	EntityUUID = entityUUID;
}

void Engine::Component::Shutdown()
{
	return;
}

void Engine::Component::Update()
{
}

void Engine::Component::Serialise(std::string Fname)
{
	std::ofstream f;
	f.open(Fname);
	if (!f.is_open())
		return;
	f << (void*)this;
	f.close();
}

std::string Engine::Component::GetName()
{
	return "ERROR!";
}

std::vector<const type_info*> Engine::Component::GetRequieredComponents()
{
	return {};
}
