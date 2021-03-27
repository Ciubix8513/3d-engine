#include "Entity.h"




Engine::Entity::Entity(D3d* D3d)
{
	Name = " ";
	UUID = 1;
	//m_D3d = D3d;
}

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

void Engine::Entity::SetTag(std::string Tag)
{
	EntityTag = Tag;
	return;
}

std::string Engine::Entity::GetTag()
{
	return EntityTag;
}

bool Engine::Entity::CompareTag(std::string other)
{
	return EntityTag == other;
}

void Engine::Entity::SetUUID(ULONG NewUUID)
{
	UUID = NewUUID;
}

void Engine::Entity::Update()
{
	for (int i = 0; i < Components.size(); i++)
		Components[i]->Update();
	return;
}

Engine::Component** Engine::Entity::GetComponent(const type_info* info)
{
	for (int i = 0; i < Components.size(); i++)
		if (Components[i]->TypeID == info->hash_code())
			return &Components[i];
	return nullptr;
}

bool Engine::Entity::ContainComponent(const type_info* info)
{
	for (int i = 0; i < Components.size(); i++)
		if (Components[i]->TypeID == info->hash_code())
			return true;
	return false;
}

