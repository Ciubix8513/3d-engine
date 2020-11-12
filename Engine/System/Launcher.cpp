#include "Launcher.h"
#include "System.h"
#include "Math/EngineMath.h"
#include "../ECS/Entity.h"
using namespace EngineMath; 
using namespace Engine;


int main()
{

	
	Entity A;
//	A.AddComponent<int>();
	A.AddComponent<MeshComponent>();
	MeshComponent* mesh = A.GetComponent<MeshComponent>();
	mesh->loadMeshFromFile("../Engine/data/Sphere.obj");
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

