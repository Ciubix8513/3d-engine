#include "Scene.h"

void Engine::Scene::AddEntity(string name)
{
	Entity A(m_D3d);
	A.AddComponent<Transform>();
	A.Name = name;
	A.Transform = A.GetComponent<Transform>();
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

bool Engine::Scene::RenderSceneFromCameraPtr(CameraComponent** Camera)
{
	vector<Entity*> Objects; //Vector of all entities that need to be rendered
	for (size_t i = 0; i < Entities.size(); i++)//Getting all objects to render
		for (size_t j = 0; j < RenderingComponents.size(); j++) 
			if (Entities[i].ContainComponent(RenderingComponents[j]))
			{
				Objects.push_back(&Entities[i]);
				break;
			}
	Entity** OrderedObjects = Objects.data();
	MaterialComponent*** MatComps = new MaterialComponent* * [Objects.size()]; // Ah yes triple pointers
	//Getting all components
	for (size_t i = 0; i < Objects.size(); i++)
		MatComps[i] = OrderedObjects[i]->GetComponent<MaterialComponent>();
	//Sorting
	bool sorted = false;
	Entity* EntityBuffer;
	MaterialComponent** MaterialComponentBuffer;
	while (!sorted)
	{
		for (size_t i = 0; i < Objects.size(); i++)
		{
			if (
				//Check rendering order
				(
					(*MatComps[i])->GetRenderingOrder()
		>
					(*MatComps[i + 1])->GetRenderingOrder()
					)
				||
				//Check which object is closer
				(
					((*OrderedObjects[i]->Transform)->GetAbsolutePosition() - (*(*Camera)->m_Transform)->GetAbsolutePosition()).Length()
	>
					((*OrderedObjects[i + 1]->Transform)->GetAbsolutePosition() - (*(*Camera)->m_Transform)->GetAbsolutePosition()).Length()
					)
				) 
			{
				sorted = false;
				goto Sorting;
			}
		}

		sorted = true;
		goto Sorted;
		Sorting:
		for (size_t i = 0; i < Objects.size() - 1; i++)
		{
			if (
				//Check rendering order
				(
					(*MatComps[i])->GetRenderingOrder()
		>
					(*MatComps[i + 1])->GetRenderingOrder()
					)
				||
				//Check which object is closer
				(
					((*OrderedObjects[i]->Transform)->GetAbsolutePosition() - (*(*Camera)->m_Transform)->GetAbsolutePosition()).Length()
	>
					((*OrderedObjects[i + 1]->Transform)->GetAbsolutePosition() - (*(*Camera)->m_Transform)->GetAbsolutePosition()).Length()
					)
				)
			{
				EntityBuffer = OrderedObjects[i + 1];
				MaterialComponentBuffer = MatComps[i + 1];
				OrderedObjects[i + 1] = OrderedObjects[i];
				MatComps[i + 1] = MatComps[i];
				OrderedObjects[i] = EntityBuffer;
				MatComps[i] = MaterialComponentBuffer;
			}
		}
		Sorted:
	}



	return true;
}



