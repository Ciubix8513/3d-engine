#include "MeshComponent.h"



void Engine::MeshComponent::Shutdown()
{
	ShutDownBuffers();
	return;
}
Engine::MeshComponent::MeshComponent()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

void Engine::MeshComponent::ShutDownBuffers()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	return;
}


void Engine::MeshComponent::InitBuffers()
{
	
	D3D11_BUFFER_DESC  indexBuffDesc;
	D3D11_BUFFER_DESC vertexBuffDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA  indexData;
	HRESULT result;
	


	vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffDesc.ByteWidth = sizeof(vertex) * m_Model.vertexCount;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;
	

	if(type == Dynamic) // If dynamic type override some params
	{
		vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	vertexData.pSysMem = m_Model.vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = (*m_D3dPtr)->getDevice()->CreateBuffer(&vertexBuffDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result)) 
	{
		throw std::exception("Failed to create vertex buffer");
		return;
	}

	indexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBuffDesc.ByteWidth = sizeof(unsigned long) * m_Model.indexCount;
	indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.CPUAccessFlags = 0;
	indexBuffDesc.MiscFlags = 0;
	indexBuffDesc.StructureByteStride = 0;

	indexData.pSysMem = m_Model.indecies;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (type == Dynamic) // If dynamic type override some params
	{
		indexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	result = (*m_D3dPtr)->getDevice()->CreateBuffer(&indexBuffDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		throw std::exception("Failed to create index buffer");
		return;
	}
	return;
}


ID3D11Buffer** Engine::MeshComponent::GetVertexBufferPtr()
{
	return &m_vertexBuffer;
}

ID3D11Buffer** Engine::MeshComponent::GetIndexBufferPtr()
{
	return &m_indexBuffer;
}

std::vector<const type_info*> Engine::MeshComponent::GetRequieredComponents()
{
	std::vector<const type_info*> a = { &typeid(Transform) };
	return a;
}

void Engine::MeshComponent::Initialise(std::vector<Component**> c, D3d** d3d)
{
	m_D3dPtr = d3d;
	transform = (Transform**)&c[0];
	
	return;
}

void Engine::MeshComponent::Render()
{
	//If there're any changes to the mesh, update the buffer
	if (m_changedMesh)
	{
		m_changedMesh = false;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT res;
		vertex* vertexPtr;
		unsigned long* indexPtr;

		//Mapping vertex buffer
		res = (*m_D3dPtr)->getDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(res))
		{
			throw std::exception("Failed to map vertex buffer");
			return;
		}
		vertexPtr = (vertex*)mappedResource.pData;
		//Setting the vertices
		memcpy(vertexPtr, (void*)m_Model.vertices, m_Model.vertexCount * sizeof(vertex));

		//Unmapping vertex buffer
		(*m_D3dPtr)->getDeviceContext()->Unmap(m_vertexBuffer, 0);


		//Mapping index buffer
		res = (*m_D3dPtr)->getDeviceContext()->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(res))
		{
			throw std::exception("Failed to map index buffer");
			return;
		}
		indexPtr = (unsigned long*)mappedResource.pData;
		//Setting the indecies
		memcpy(indexPtr, (void*)m_Model.indecies, m_Model.indexCount * sizeof(unsigned long));

		//Unmapping index buffer
		(*m_D3dPtr)->getDeviceContext()->Unmap(m_indexBuffer, 0);

	}
	//Setting vertex buffer
	(*m_D3dPtr)->getDeviceContext()->IASetVertexBuffers(0, 1,&m_vertexBuffer, 0, 0);
	//Setting index buffer
	(*m_D3dPtr)->getDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Setting primitive topology
	(*m_D3dPtr)->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

Engine::MeshComponent::MeshType Engine::MeshComponent::GetMeshType()
{
	return type;
}

void Engine::MeshComponent::SetMeshType(MeshType type1)
{
	if (type == type1)
		return;
	type = type1;
	if (m_vertexBuffer)
	{
		InitBuffers(); //if changing mesh type reinitialise buffers
		ShutDownBuffers();
	}
	return;
}

void Engine::MeshComponent::SetMesh(Mesh mesh)
{
	m_Model = mesh;	
	m_changedMesh = true;
	if (type == Static) //If mesh is static  create new buffers
	{	
		m_changedMesh = false;
		ShutDownBuffers();
		InitBuffers();
		return;
	}
	if (!m_vertexBuffer || !m_indexBuffer)	
		InitBuffers();	
	return;
}

bool Engine::MeshComponent::vertex::operator==(vertex& b)
{
	int e = 1;
	vertex* a = this;
	e *= (int)(a->color.x * 10000) == (int)(b.color.x * 10000);
	e *= (int)(a->color.y * 10000) == (int)(b.color.y * 10000);
	e *= (int)(a->color.z * 10000) == (int)(b.color.z * 10000);
	e *= (int)(a->color.w * 10000) == (int)(b.color.w * 10000);
	e *= (int)(a->normal.x * 10000) == (int)(b.normal.x * 10000);
	e *= (int)(a->normal.z * 10000) == (int)(b.normal.z * 10000);
	e *= (int)(a->normal.y * 10000) == (int)(b.normal.y * 10000);
	e *= (int)(a->position.x * 10000) == (int)(b.position.x * 10000);
	e *= (int)(a->position.y * 10000) == (int)(b.position.y * 10000);
	e *= (int)(a->position.z * 10000) == (int)(b.position.z * 10000);
	e *= (int)(a->UV.x * 10000) == (int)(b.UV.x * 10000);
	e *= (int)(a->UV.y * 10000) == (int)(b.UV.y * 10000);
	return e == 1;
}
