#include "Scene.h"

void Engine::Scene::AddEntity()
{
	Entity A(m_D3d);
	A.AddComponent<Transform>();
	A.UUID = Entitys.size();
	Entitys.insert(Entitys.begin(),A);
}

void Engine::Scene::DeleteEntity(Entity* entity)
{	
	entity->Destroy(); //Might not work
	Entitys.erase(Entitys.find(*entity));
}

void Engine::Scene::SerialiseScene(string Path)
{

}

void Engine::Scene::DeSerialiseScene(string Path)
{

}

bool Engine::Scene::GetActiveState()
{
	return Active;
}

void Engine::Scene::SetActiveState(bool state)
{
	Active = state;
	return;
}

const Engine::Entity& Engine::Scene::GetEntityByName(string Name)
{
	for (set<Entity>::iterator i = Entitys.begin(); i != Entitys.end(); i++) {
		const Entity& A =  (*i);
		if (A.Name == Name)
			return A;
	}
	return nullptr;
}

const Engine::Entity& Engine::Scene::GetEntityByUUID(unsigned long UUID)
{
	for (set<Entity>::iterator i = Entitys.begin(); i != Entitys.end(); i++) {
		const Entity& A = (*i);
		if (A.UUID == UUID)
			return A;
	}
	return nullptr;
}

const Engine::Entity& Engine::Scene::GetEntityByTag(string Tag)
{
	for (set<Entity>::iterator i = Entitys.begin(); i != Entitys.end(); i++) {
		const Entity& A = (*i);
		if (A.CompareTag(Tag))
			return A;
	}
	return nullptr;
}

const Engine::Entity& Engine::Scene::GetEntityByName_Tag(string Name, string Tag)
{
	return nullptr;
}



