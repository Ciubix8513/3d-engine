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
		void InitBuffers(ID3D11Device* device);
		void ShutDownBuffers();
		ID3D11Buffer** GetVertexBufferPtr();
		ID3D11Buffer** GetIndexBufferPtr();
		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component**>) override;
	
	private:
		ID3D11Buffer* m_VertexBuffer,* m_IndexBuffer;
		Mesh m_Model;
		Transform** transform;

	};
}
#endif