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
		void AddEntity(std::string name = "");

#pragma region Delete Entity
		void DeleteEntityByPointer(Entity* entity);
		void DeleteEntityByUUID(ULONG UUID);
#pragma endregion

		Scene(D3d*);


		void ShutDown();
		void Update();

		void SerialiseScene(std::string Path);
		void DeSerialiseScene(std::string Path);
		bool GetActiveState();
		void SetActiveState(bool state);
		void ResetUUIDs();

#pragma region Get entity functions
		 Engine::Entity* GetEntityByName(std::string Name);
		 Engine::Entity* GetEntityByUUID(unsigned long UUID);
		 Engine::Entity* GetEntityByTag(std::string Tag);
		 Engine::Entity* GetEntityByName_Tag(std::string Name,std::string Tag);
#pragma endregion

#pragma region Rendering
		 bool RenderSceneFromCameraPtr(CameraComponent** Camera);
#pragma endregion

	private:
		std::vector<Entity> Entities;
		D3d** m_D3d; 
		bool Active;
		//Quick and dirty, but should work
		ULONG UUIDcounter = 1;
		const std::vector <const type_info*> RenderingComponents = { &typeid(MaterialComponent) }; // For expandability probably won't work but I'll still add this
	};
}

#endif

