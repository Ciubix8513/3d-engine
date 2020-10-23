#include "Entity.h"



void Engine::Entity::Destroy()
{
	for (int i = 0; i < Components.size(); i++)	
		Components[i].Shutdown();
	
}

void Engine::Entity::A()
{
	AddComponent<Transform>();
}

template<typename T>
void Engine::Entity::AddComponent()
{
	//Check if there's a component type T
	if (!std::is_base_of<Component, T>::value)
	{
		throw std::exception("Wrong component type");
		return;
	}
	T Comp;
	Components.push_back(Comp);
	Components[Components.size() - 1].Initialise();
}

template<typename T>
void Engine::Entity::RemoveComponent()
{
	//Check if there's a component type T
	if (!std::is_base_of<Component, T>::value)
	{
		throw std::exception("Wrong component type");
		return;
	}
	for (int i = 0; i < Components.size(); i++)	
		if (typeid(Components[i]) == typeid(T))
		{
			Components[i].Shutdown(); 
			Components.erase(Components.begin() + i);
			return;
		}	

}

template<typename T>
T* Engine::Entity::GetComponent()
{
	//Check if there's a component type T
	if (!std::is_base_of<Component, T>::value)
	{
		throw std::exception("Wrong component type");
		return;
	}
	for (int i = 0; i < Components.size(); i++)
		if (typeid(Components[i]) == typeid(T))
			return &Components[i];
}

