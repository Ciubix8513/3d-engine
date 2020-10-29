#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "General/Transform.h"
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
	public:
#pragma region Component functions
		template <typename T>
		void AddComponent(); 
		template <typename T>
		void RemoveComponent();
		template <typename T>
		T* GetComponent();
#pragma endregion
		
		void Destroy();
	private:
		void Update();
		void A();
		
	public:
		std::string Name;
	private:
		std::vector<Engine::Component> Components;	
		long UUID;
		
	};
	
	

}


#endif
