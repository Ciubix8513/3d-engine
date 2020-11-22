#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include "../Graphics/D3d.h"
namespace Engine
{
	class Component 
	{
		friend class Entity;

	public:
		Component();
		virtual void Initialise(D3d* directX);
		virtual void Initialise(std::vector<Component*>);
		virtual void Shutdown();
		virtual void Update();
		size_t TypeID;
		std::vector<const type_info*> RequieredComponents;
	private:
		D3d* m_D3d;
	};
}

#endif 

