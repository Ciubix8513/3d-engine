#include "Scene.h"

void Engine::Scene::AddEntity(string name)
{
	Entity A(m_D3d);
	A.AddComponent<Transform>();
	A.Name = name;
	Entitys.push_back(A);	
	if (UUIDcounter == 0)
		ResetUUIDs();
	else
	{
		Entitys[Entitys.size() - 1].SetUUID(UUIDcounter);
		UUIDcounter++;
	}
	return;
}

void Engine::Scene::DeleteEntityByPointer(Entity* entity)
{	
	entity->Destroy(); //Might not work
	for (auto i = Entitys.begin(); i != Entitys.end(); i++)	
		if ((*i).UUID == entity->UUID)		
			Entitys.erase(i);
	return;	
}
void Engine::Scene::DeleteEntityByUUID(ULONG UUID)
{
	auto It = Entitys.begin();
	for (auto i = Entitys.begin(); i != Entitys.end(); i++)
		if ((*i).UUID == UUID) 
		{
			(*i).Destroy();
			Entitys.erase(i);
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
	for (UUIDcounter = 1; UUIDcounter < Entitys.size() + 1; UUIDcounter++)
			Entitys[UUIDcounter - 1].SetUUID(UUIDcounter);
	return;
}


Engine::Entity* Engine::Scene::GetEntityByName(string Name)
{
	for (size_t i = 0; i < Entitys.size(); i++)
		if (Entitys[i].Name == Name)
			return &Entitys[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByUUID(unsigned long UUID)
{
	for (size_t i = 0; i < Entitys.size(); i++)
		if (Entitys[i].UUID == UUID)
			return &Entitys[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByTag(string Tag)
{
	for (size_t i = 0; i < Entitys.size(); i++)
		if (Entitys[i].CompareTag(Tag))
			return &Entitys[i];
	return nullptr;
}
Engine::Entity* Engine::Scene::GetEntityByName_Tag(string Name, string Tag)
{
	for (size_t i = 0; i < Entitys.size(); i++)
		if (Entitys[i].Name == Name && Entitys[i].CompareTag(Tag))
			return &Entitys[i];
	return nullptr;
}



