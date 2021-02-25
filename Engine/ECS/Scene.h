#pragma once
#ifndef _ENTITY_CONTAINER_H_
#define _ENTITY_CONTAINER_H_
#include "Entity.h"
#include <set>
namespace Engine 
{
	class Scene
	{
	public:
		void AddEntity();
		void DeleteEntity(Entity* entity);
		void SerialiseScene(string Path);
		void DeSerialiseScene(string Path);
		bool GetActiveState();
		void SetActiveState(bool state);
		void ResetUUIDs();

#pragma region Get entity functions
		 Engine::Entity* GetEntityByName(string Name);
		 Engine::Entity* GetEntityByUUID(unsigned long UUID);
		 Engine::Entity* GetEntityByTag(string Tag);
		 Engine::Entity* GetEntityByName_Tag(string Name,string Tag);
#pragma endregion

	private:
		vector<Entity> Entitys;
		D3d* m_D3d; 
		bool Active;
		//Quick and dirty, but should work
		unsigned long int UUIDcounter = 1;
	};
}

#endif

