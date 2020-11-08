#include "Entity.h"



Engine::Entity::Entity()
{
	Name = " ";
	UUID = 1;
}

void Engine::Entity::Destroy()
{
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->Shutdown();
		delete Components[i];
	}
	throw "Destroy object" + UUID ;
}

void Engine::Entity::Update()
{
	for (int i = 0; i < Components.size(); i++)
		Components[i]->Update();
	return;
}

