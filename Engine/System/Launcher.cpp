#include "Launcher.h"
#include "System.h"
#include "Math/EngineMath.h"
#include "../ECS/Entity.h"
using namespace EngineMath; 
using namespace Engine;


int main()
{
	Entity a = Entity();
	a.AddComponent<Transform>();
	(*a.GetComponent<Transform>())->testData = "This is a test";
	a.AddComponent<MeshComponent>();



	auto b = a.ContainComponent<MeshComponent>();
	auto C = a.ContainComponent<Transform>();
	cout << " ";
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

