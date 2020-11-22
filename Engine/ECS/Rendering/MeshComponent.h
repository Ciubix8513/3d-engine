#ifndef _MESH_COMPONENT_H_
#define _MESH_COMPONENT_H_
#pragma warning(disable : 4996)
#include "../Component.h"
#include "../General/Transform.h"
#include "../../System/Math/EngineMath.h"
#include <string>
#include <d3d11.h>
#include <fstream>
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
		void Shutdown() override;
		MeshComponent();
		bool loadMeshFromObjFile(string path);
		void SerialiseMesh(string path);
		Mesh DeSerialiseMesh(string path);
		void InitBuffers(ID3D11Device* device);
		void ShutDownBuffers();
		ID3D11Buffer** GetVertexBufferPtr();
		ID3D11Buffer** GetIndexBufferPtr();
	public:
		Mesh m_Model;
		vector<const type_info*>RequieredComponents = { &typeid(Transform) };
	private:
		ID3D11Buffer* m_VertexBuffer,* m_IndexBuffer;
		
	};
}
#endif