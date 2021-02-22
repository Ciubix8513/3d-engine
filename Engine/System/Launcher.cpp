#include "Launcher.h"
#include "System.h"
#include "Math/EngineMath.h"
#include "../ECS/Scene.h"
using namespace EngineMath; 
using namespace Engine;


int main()
{
	Scene scene;
	scene.AddEntity();
	scene.AddEntity();
	

	cout << ' ';
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

