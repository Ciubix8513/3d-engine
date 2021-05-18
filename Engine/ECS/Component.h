#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <fstream>
#include "../Graphics/D3d.h"
namespace Engine
{
	
	class Component abstract
	{
		friend class Entity;

	public:
		Component();
		//virtual void Initialise(D3d* directX);
		virtual void Initialise(std::vector<Component*>,D3d** d3d, ULONG EntityUUID);
		virtual void Shutdown();
		virtual void Update();
		virtual void Serialise(std::string Fname);
		virtual std::string GetName();
		virtual std::vector< const type_info*> GetRequieredComponents();
		size_t TypeID;
		
//private:
	protected:
		D3d** m_D3dPtr;
		ULONG EntityUUID;
	};
}

#endif 

