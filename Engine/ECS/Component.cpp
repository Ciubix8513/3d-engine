#include "Component.h"

Engine::Component::Component()
{
	
}

void Engine::Component::Initialise()
{
	std::cout << "A\n";
	
	return;
}

void Engine::Component::Initialise(std::vector<Component*>)
{
}

void Engine::Component::Shutdown()
{
	return;
}

void Engine::Component::Update()
{
}
