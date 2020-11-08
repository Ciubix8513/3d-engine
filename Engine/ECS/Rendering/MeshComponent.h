#ifndef _MESH_COMPONENT_H_
#define _MESH_COMPONENT_H_
#include "../Component.h"
#include <iostream>
#include <vector>
#include <string>
#include <d3d11.h>
#include <fstream>
#include "../../System/Math/EngineMath.h"
using namespace EngineMath;
using namespace std;

namespace Engine
{

	class MeshComponent : public Component
	{
	private:
		struct Count
		{
			int vertexCount;
			int indexCount;
		};
	public:


		struct vertex
		{
			Vector3 position;
			Vector4 color;
			Vector2 UV;
			Vector3 normal;

			bool operator==(vertex& other);

		};
		struct Mesh
		{
			vertex* vertices;
			unsigned long* indecies;
			int vertexCount;
			int indexCount;
		};

	public:
		MeshComponent();
		bool loadMeshFromFile(string path);
		void SerialiseMesh(string path);
		Mesh DeSerialiseMesh(string path);
	public:
		Mesh Model;
		
	};
}
#endif