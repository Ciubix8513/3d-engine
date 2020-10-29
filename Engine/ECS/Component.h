#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_


namespace Engine
{
	class Component 
	{
	public:
		void Initialise();
		void Shutdown();
		void Update();
	};
}
#endif 

