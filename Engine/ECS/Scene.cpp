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


