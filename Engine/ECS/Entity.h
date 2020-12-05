#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "General/Transform.h"
#include "Rendering/MeshComponent.h"
#include <vector>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <exception>


namespace Engine
{
	using namespace Engine;
	class Entity
	{
		friend class Scene;
	public:
		Entity(D3d* D3d);Entity();

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
			//Check for required components
			auto *Comp = new T;			
			Components.push_back((Component*)Comp);
			Components[Components.size() - 1]->TypeID = typeid(T).hash_code();
			vector<Component*> InitComp;
			for (int i = 0; i < Components[Components.size() - 1]->RequieredComponents.size(); i++)
			{
				if (ContainComponent(Components[Components.size() - 1]->RequieredComponents[i]))
					InitComp.push_back(*GetComponent(Components[Components.size() - 1]->RequieredComponents[i]));
				else
					AddComponent< decltype(& Components[Components.size() - 1]->RequieredComponents[i])>();
			}
			Components[Components.size() - 1]->Initialise(InitComp);
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
		T** GetComponent()
		{
			//Check if there's a component type T
			if (!std::is_base_of<Component, T>::value)
			{
				throw std::exception("Wrong component type");

			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
					return &(T*)Components[i];
			return nullptr;
		};

		Component** GetComponent(const type_info* info);
		
		


		template <typename T>
		bool ContainComponent() 
		{
			typeid(int);
			//Check if there's a component type T
			if (!std::is_base_of<Component, T>::value)
			{
				throw std::exception("Wrong component type");

			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
					return true;
			return false;
		};
		bool ContainComponent(const type_info* info);


#pragma endregion
	private:
		D3d* m_D3d;

	};



}


#endif
