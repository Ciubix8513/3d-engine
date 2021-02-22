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
#pragma region Get entity functions
		const Engine::Entity& GetEntityByName(string Name);
		 const Engine::Entity& GetEntityByUUID(unsigned long UUID);
		 const Engine::Entity& GetEntityByTag(string Tag);
		 const Engine::Entity& GetEntityByName_Tag(string Name,string Tag);
#pragma endregion

	private:
		set<Entity> Entitys;
		D3d* m_D3d; 
		bool Active;
	};
}

#endif

