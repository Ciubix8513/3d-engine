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
	private:
		set<Entity> Entitys;
		D3d* m_D3d; 
		bool Active;
	};
}

#endif

