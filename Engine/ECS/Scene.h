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
		void AddEntity(string name = "");

#pragma region Delete Entity
		void DeleteEntityByPointer(Entity* entity);
		void DeleteEntityByUUID(ULONG UUID);
#pragma endregion

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
		ULONG UUIDcounter = 1;
	};
}

#endif

