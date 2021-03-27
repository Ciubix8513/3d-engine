#include "Launcher.h"
#include "System.h"
#include "Math/EngineMath.h"
#include "../ECS/Scene.h"
using namespace EngineMath; 
using namespace Engine;


int main()
{
	/*Scene scene;
	scene.AddEntity("A");
	scene.AddEntity("B");	

	auto Entity0 = scene.GetEntityByUUID(1);
	Entity0->SetTag("123");
	auto Entity1 = scene.GetEntityByTag("123");
	auto Entity2 = scene.GetEntityByUUID(2);*/

	

	std::cout << ' ';
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

