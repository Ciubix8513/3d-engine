#include "Launcher.h"
#include "System.h"
#include "Math/EngineMath.h"
#include "../ECS/Entity.h"
using namespace EngineMath; 
using namespace Engine;


int main()
{
	auto d = std::is_base_of<Component, Transform>::value;
	Entity a;
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

