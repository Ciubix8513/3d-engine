#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>
namespace Engine
{
	class Component 
	{
		friend class Entity;

	public:
		Component();
		virtual void Initialise();
		virtual void Initialise(std::vector<Component*>);
		virtual void Shutdown();
		virtual void Update();
		size_t TypeID;
		std::vector<const type_info*> RequieredComponents;
		
	};
}

#endif 

