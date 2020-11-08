#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <iostream>

namespace Engine
{
	class Component 
	{
		
	public:
		Component();
		virtual void Initialise();
		virtual void Shutdown();
		virtual void Update();
		size_t TypeID;
		friend class Entity;
		
	};
}

#endif 

