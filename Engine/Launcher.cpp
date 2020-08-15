#include "Launcher.h"
#include <iostream>
#include "System.h"
#include "System.h"


	int main()
	{
		System* system;
		bool result;
		system = new System();
		if (!system)
			return 0;
		result = system->Init();
		if (result)
			system->Run();
		system->ShutDown();
		delete system;
		system = 0;
		return 1;
	}