#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "General/Transform.h"
#include "Rendering/MeshComponent.h"
#include <vector>
#include <string>
#include <type_traits>
#include <exception>
#include <typeinfo>

namespace Engine
{
	using namespace Engine;
	class Entity
	{
		friend class Scene;
	public:
		Entity();


		void Destroy();
	private:
		void Update();


	public:
		std::string Name;
	private:
		std::vector<Component*> Components;
		long UUID;


	public:
#pragma region Component functions
		template <typename T>
		void AddComponent()
		{
			//Check if there's a component type T
			if (!std::is_base_of<Component, T>::value)
			{
				throw std::exception("Wrong component type");
				return;
			}
			
			T *Comp = new T;
			Comp->TypeID = typeid(T).hash_code();
			Components.push_back(Comp);
			(Components[Components.size() - 1])->Initialise();
		};

		template <typename T>
		void RemoveComponent()
		{
			//Check if there's a component type T
			if (!std::is_base_of<Component, T>::value)
			{
				throw std::exception("Wrong component type");
				return;
			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
				{
					Components[i].Shutdown(); // .Shutdown();
					delete Components[i];
					Components.erase(Components.begin() + i);
					return;
				}

		};
		template <typename T>
		T* GetComponent() {
			//Check if there's a component type T
			if (!std::is_base_of<Component, T>::value)
			{
				throw std::exception("Wrong component type");

			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
					return (T*)Components[i];
			return nullptr;
		};

#pragma endregion

	};



}


#endif
