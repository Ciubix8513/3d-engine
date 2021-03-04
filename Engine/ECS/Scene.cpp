#include "Scene.h"

void Engine::Scene::AddEntity(string name)
{
	Entity A(m_D3d);
	A.AddComponent<Transform>();
	A.Name = name;
	Entities.push_back(A);	
	if (UUIDcounter == 0)
		ResetUUIDs();
	else
	{
		Entities[Entities.size() - 1].SetUUID(UUIDcounter);
		UUIDcounter++;
	}
	return;
}

void Engine::Scene::DeleteEntityByPointer(Entity* entity)
{	
	entity->Destroy(); //Might not work
	for (auto i = Entities.begin(); i != Entities.end(); i++)	
		if ((*i).UUID == entity->UUID)		
			Entities.erase(i);
	return;	
}
void Engine::Scene::DeleteEntityByUUID(ULONG UUID)
{
	auto It = Entities.begin();
	for (auto i = Entities.begin(); i != Entities.end(); i++)
		if ((*i).UUID == UUID) 
		{
			(*i).Destroy();
			Entities.erase(i);
		}
	return;
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

//Just A quick function to reset UUIDs just in case
void Engine::Scene::ResetUUIDs()
{	
	for (UUIDcounter = 1; UUIDcounter < Entities.size() + 1; UUIDcounter++)
			Entities[UUIDcounter - 1].SetUUID(UUIDcounter);
	return;
}


Engine::Entity* Engine::Scene::GetEntityByName(string Name)
{
	for (size_t i = 0; i < Entities.size(); i++)
		if (Entities[i].Name == Name)
			return &Entities[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByUUID(unsigned long UUID)
{
	for (size_t i = 0; i < Entities.size(); i++)
		if (Entities[i].UUID == UUID)
			return &Entities[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByTag(string Tag)
{
	for (size_t i = 0; i < Entities.size(); i++)
		if (Entities[i].CompareTag(Tag))
			return &Entities[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByName_Tag(string Name, string Tag)
{
	for (size_t i = 0; i < Entities.size(); i++)
		if (Entities[i].Name == Name && Entities[i].CompareTag(Tag))
			return &Entities[i];
	return nullptr;
}

bool Engine::Scene::RenderScene()
{
	vector<Entity*> Objects;
	for (size_t i = 0; i < Entities.size(); i++)//Getting all objects to render
		if (Entities[i].ContainComponent<Engine::MaterialComponent>())
			Objects.push_back(&Entities[i]);
	

	return true;
}



