#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "General/Transform.h"
#include "Rendering/MaterialComponent.h"

#include "Rendering/CameraComponent.h"
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
		Entity(D3d* D3d); Entity();
		void Destroy();
#pragma region Tag funcs
		void SetTag(std::string Tag);
		std::string GetTag();
		bool CompareTag(std::string other);

#pragma endregion
#pragma region Component functions
		template <typename T>
		void AddComponent()
		{
			try
			{
				//Check if there's a component type T
				if (!std::is_base_of<Component, T>::value)
				{
					throw std::exception("Wrong component type");
					return;
				}
				//Check for required components
				T* Comp = new T;
				Components.push_back((Component*)Comp);
				Components[Components.size() - 1]->TypeID = typeid(T).hash_code();
				std::vector<Component**> InitComp;

				std::vector<const type_info*> Comps = Components[Components.size() - 1]->GetRequieredComponents();

				for (int i = 0; i < Comps.size(); i++)
				{
					InitComp.push_back(GetComponent(Comps[i]));
					if (InitComp[i] == nullptr)
					{
						std::string str = "Failed to add \"";
						str += (*Comps[i]).name();
						str += "\" to the list, component is not added";
						throw std::exception(str.c_str());
						RemoveComponent<T>();
						return;
					}
				}
				Components[Components.size() - 1]->Initialise(InitComp);
			}
			catch (std::exception& e)
			{
				std::cerr << "Got std::exception: " << e.what() << std::endl; //TODO: send to internal erorr handling system;			
			}
		};
		template <typename T>
		void RemoveComponent()
		{
			//Check if there's a component type T
			try
			{
				if (!std::is_base_of<Component, T>::value)
				{
					throw std::exception("Wrong component type");
					return;
				}
			}
			catch (std::exception& e)
			{
				std::cerr << "Got exceprion: " << e.what() << std::endl; //TODO: send to internal erorr handling system;			
			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
				{
					Components[i]->Shutdown();
					delete Components[i];
					Components.erase(Components.begin() + i);
					return;
				}

		};
		template <typename T>
		T** GetComponent()
		{
			//Check if there's a component type T
			try
			{
				if (!std::is_base_of<Component, T>::value)
				{
					throw std::exception("Wrong component type");
				}
			}
			catch (std::exception& e)
			{
				std::cerr << "Got exceprion: " << e.what() << std::endl; //TODO: send to internal erorr handling system;			
			}
			for (int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
					return (T**)&Components[i];
			return nullptr;
		};
		Component** GetComponent(const type_info* info);
		template <typename T>
		bool ContainComponent()
		{
			auto a = typeid(T).name();
			//Check if there's a component type T
			try
			{
				if (!std::is_base_of<Component, T>::value)
				{
					throw std::exception("Wrong component type");
					return false;
				}
			}
			catch (std::exception& e)
			{
				std::cerr << "Got exceprion: " << e.what() << std::endl; //TODO: send to internal erorr handling system;			
			}
			for (unsigned int i = 0; i < Components.size(); i++)
				if (Components[i]->TypeID == typeid(T).hash_code())
					return true;
			return false;
		};
		bool ContainComponent(const type_info* info);
#pragma endregion		
	private:
		void SetUUID(ULONG NewUUID);
		void Update();
		bool change;
		std::string Name;
		std::vector<Component*> Components;
		ULONG UUID;
		std::string EntityTag;
	public:
		Engine::Transform** Transform;

	};
};


#endif
