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
	auto it = Entitys.begin() + entity->UUID;
	entity->Destroy();
	Entitys.erase(it);
}

void Engine::Scene::SerialiseScene(string Path)
{
}

void Engine::Scene::DeSerialiseScene(string Path)
{
}


