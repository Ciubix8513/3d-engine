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
//using namespace std;

namespace Engine
{

	class MeshComponent : public Component
	{ 
		
		friend class MaterialComponent;
	public:
		enum MeshType
		{
			Dynamic,Static
		};
		struct vertex
		{
			Vector3 position;
			Vector4 color;
			Vector2 UV;
			Vector3 normal;			

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
		void InitBuffers();
		void ShutDownBuffers();
		ID3D11Buffer** GetVertexBufferPtr();
		ID3D11Buffer** GetIndexBufferPtr();
		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component**>, D3d* d3d) override;

		void Render();

		MeshType GetMeshType();
		void SetMeshType(MeshType type);
		void SetMesh(Mesh mesh);

	private:
		ID3D11Buffer* m_vertexBuffer,* m_indexBuffer;
		Mesh m_Model;
		Transform** transform;
		MeshType type;
		bool m_changedMesh;
	};
}
#endif