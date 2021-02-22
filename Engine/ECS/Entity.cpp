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

bool Engine::Entity::GetChagedState()
{
	return change;
}

void Engine::Entity::SetChangedState(bool state)
{
	change = state;
	return;

	
}

bool Engine::Entity::operator<(const Entity& other) const
{
	
	return UpdateOrder <  other.UpdateOrder;
}

bool Engine::Entity::operator>=(const Entity other)
{
	return UpdateOrder < other.UpdateOrder;
}

bool Engine::Entity::operator()(const Entity& _Left, const Entity& _Right) const
{
	return _Left.UpdateOrder < _Right.UpdateOrder;
}

