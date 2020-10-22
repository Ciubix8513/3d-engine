#include "Model.h"

Model::Model()
{
	m_vertexBuf = 0;
	m_indexBuf = 0;
	m_texture = 0;
}

Model::Model(Model&)
{
}

Model::~Model()
{
}

Matrix4x4 Model::Getobjectmatrix()
{	
	
	return TransformationMatrix(m_position, m_Rotaion, m_scale);;
}



Model::Mesh Model::loadMeshFromFile(char* fileName)
{
	Mesh mesh = Mesh();	
	ifstream file(fileName);
	string line;
	string fstChar = "01";

	vector<string> wipVec;
	if (!file.is_open()) 
	{
		cout << "Failed to open file";
		return Mesh();
	}
	cout << "opened file"<<endl;
	int vCount = 0 , vtCount = 0, vnCount = 0, fCount = 0;

	//vector<Vector2> a;
	

	vector<Vector3> Vertices, Normals;
	vector<vertex> Tris;
	vector<Vector2> UVs;
	while (getline(file, line))
	{

		if (line[0] == 'v' && line[1] == ' ')
		{

			string currentFloat;
			int XYZ = 0;
			Vector3 currentVec;
			for (int i = 2; i < line.length(); i++)
			{
				if (line[i] != ' ')
					currentFloat += line[i];
				else
				{
					if (XYZ == 0)
						currentVec.x = (float)atof(currentFloat.c_str());
					if (XYZ == 1)
						currentVec.y = (float)atof(currentFloat.c_str());
					XYZ++;
					currentFloat = ' ';
				}
				if (XYZ == 2)
					currentVec.z = (float)atof(currentFloat.c_str());
			}

			Vertices.push_back(currentVec);

		}

		if (line[0] == 'v' && line[1] == 't')
		{

			string currentFloat;
			int XY = 0;
			Vector2 currentVec;
			for (int i = 3; i < line.length(); i++)
			{
				if (line[i] != ' ')
					currentFloat += line[i];
				else
				{
					if (XY == 0)
						currentVec.x = (float)atof(currentFloat.c_str());
					XY++;
					currentFloat = ' ';
				}
				if (XY == 1)
					currentVec.y = (float)atof(currentFloat.c_str());
			}

			UVs.push_back(currentVec);

		}

		if (line[0] == 'v' && line[1] == 'n')
		{
		
			string currentFloat;
			int XYZ = 0;
			Vector3 currentVec;
			for (int i = 3; i < line.length(); i++)
			{
				if (line[i] != ' ')
					currentFloat += line[i];
				else
				{
					if (XYZ == 0)
						currentVec.x = (float)atof(currentFloat.c_str());
					if (XYZ == 1)
						currentVec.y = (float)atof(currentFloat.c_str());
					XYZ++;
					currentFloat = ' ';
				}
				if (XYZ == 2)
					currentVec.z = (float)atof(currentFloat.c_str());
			}

			Normals.push_back(currentVec);
		}

		auto Vs = Vertices;
		auto VTs = UVs;
		auto VNs = Normals;



		if (line[0] == 'f')
		{

			vertex currentVertex;
			string curentIndex;
			int Index = 0;

			for (int i = 2; i < line.length(); i++)
			{
				if (line[i] != '/' && line[i] != ' ')
				{
					curentIndex += line[i];
				}
				else
				{
					switch (Index)
					{
					case 0:
						currentVertex.position = Vs[(UINT)atof(curentIndex.c_str()) - 1];
						Index++;
						curentIndex = ' ';
						break;
					case 1:
						currentVertex.UV = VTs[(UINT)atof(curentIndex.c_str()) - 1];
						Index++;
						curentIndex = ' ';
						break;
					case 2:
						currentVertex.normal = VNs[(UINT)atof(curentIndex.c_str()) - 1];
						Index++;
						Index = 0;
						Tris.push_back(currentVertex);
						curentIndex = ' ';
						break;
					}
				}
			}
			currentVertex.normal = VNs[(UINT)atof(curentIndex.c_str()) - 1];
			Tris.push_back(currentVertex);
		}
	}
	file.close();

	auto Faces = Tris;
	vector<vertex> FinalVertices;
	vector<unsigned long> Indecies;
	for (int i = 0; i < Faces.size(); i++)
	{
		for (int j = 0; j < Faces.size(); j++)
		{
			if (Faces[i] == Faces[j]) 
			{
				
				Indecies.push_back(j);
				FinalVertices.push_back(Faces[i]);
				goto a;
			}
		}
	a:
		cout << '\n';
	}
	Vertices.clear();
	Normals.clear();
	Tris.clear();
	UVs.clear();

	/*
	auto SetVertices = FinalVertices;
	auto Setindecies = Indecies;

	mesh.indexCount = Setindecies.size();
	mesh.vertexCount = SetVertices.size();
	mesh.indecies = new unsigned long [Setindecies.size()];
	mesh.vertices = new vertex[SetVertices.size()];
	for (size_t i = 0; i < mesh.vertexCount; i++)	
		mesh.vertices[i] = SetVertices[i];	
	for (size_t i = 0; i < mesh.indexCount; i++)	
		mesh.indecies[i] = Setindecies[i];
	*/
	
	mesh.indexCount = Indecies.size();
	mesh.vertexCount = FinalVertices.size();
	mesh.indecies = new unsigned long[Indecies.size()];
	mesh.vertices = new vertex[FinalVertices.size()];
	for (size_t i = 0; i < mesh.vertexCount; i++)
		mesh.vertices[i] = FinalVertices[i];
	for (size_t i = 0; i < mesh.indexCount; i++)
		mesh.indecies[i] = Indecies[i];

	return mesh;	
}





bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename,char* file)
{
	bool result;
	result = InitBuffers(device,file);
	if (!result)
		return false;
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) {
		cout << "Could not load texture";
		return false;
	}
	return true;
}

void Model::SetRotation(float x, float y, float z)
{	
	x:
	if (x >= 360)
	{
		x -= 360;
		goto x;
	}
	y:
	if (y >= 360)
	{
		y -= 360;
		goto y;
	}
	z:
	if (z >= 360)
	{
		z -= 360;
		goto z;
	}
	m_Rotaion = Vector3(x, y, z);

}

void Model::SetRotation(Vector3 v)
{
	
x:
	if (v.x >= 360)
	{
		v.x -= 360;
		goto x;
	}
y:
	if (v.y >= 360)
	{
		v.y -= 360;
		goto y;
	}
z:
	if (v.z >= 360)
	{
		v.z -= 360;
		goto z;
	}
	m_Rotaion =v;

}



void Model::Shutdown()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
	ShutDownBuffers(); 
	return;
}

void Model::Render(ID3D11DeviceContext* ctxt, ID3D11Device* device)
{
	RenderBuffers(ctxt);

	return;
}

Vector3 Model::GetRotation()
{
	return m_Rotaion;
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_texture->GetTexture();
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* ctxt, char* file)
{
	bool result;
	m_texture = new Texture;
	if (!m_texture)
		return false;
	result = m_texture->Init(device, ctxt, file);
	if (!result)
		return false;
	return true;
}

bool Model::InitBuffers(ID3D11Device* device, char* file)
{
	unsigned long*  indices ;
	D3D11_BUFFER_DESC  indexBuffDesc;
	D3D11_BUFFER_DESC vertexBuffDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA  indexData;
	HRESULT result;
	vertex* vertices;

	



	if (file == (char*)"No U")
	{
		Default:
		m_vertexCount = 3;
		m_indexCount = 3;

		vertices = new vertex[m_indexCount];
		if (!vertices)
			return false;
		indices = new unsigned long[m_indexCount];
		if (!indices)
			return false;
		vertices[0].color = Vector4(1, 1, 1, 1);
		vertices[0].position = Vector3(-1, -1, 0);
		vertices[0].UV = Vector2(0.0f, .7f);
		vertices[0].normal = Vector3(0.0f, 0.0f, 1.0f);

		vertices[1].color = Vector4(1, 1, 1, 1);
		vertices[1].position = Vector3(0, 1, 0);
		vertices[1].UV = Vector2(0.5f, 0.0f);
		vertices[1].normal = Vector3(0.0f, 0.0f, 1.0f);

		vertices[2].color = Vector4(1, 1, 1, 1);
		vertices[2].position = Vector3(1, -1, 0);
		vertices[2].UV = Vector2(1.0f, 0.7f);
		vertices[2].normal = Vector3(0.0f, 0.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
	}
	else 
	{
		Mesh* mesh = &loadMeshFromFile(file);
		if (!mesh) 
		{
			cout << "Failed to load mesh " << file << " loading a triangle instead";
			goto Default;
		}
		m_indexCount = mesh->indexCount;
		m_vertexCount = mesh->vertexCount;
		vertices = mesh->vertices;
		indices = mesh->indecies;

		mesh = 0;
	}


	vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffDesc.ByteWidth = sizeof(vertex) * m_vertexCount;
	vertexBuffDesc.BindFlags = 0x1L;//0x1L = bind vertex buffer
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	result = device->CreateBuffer(&vertexBuffDesc, &vertexData, & m_vertexBuf);
	if(FAILED(result))
		return false;

	indexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBuffDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBuffDesc.BindFlags = 0x2L;//0x2L = bind index buffer
	indexBuffDesc.CPUAccessFlags = 0;
	indexBuffDesc.MiscFlags = 0;
	indexBuffDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBuffDesc, &indexData, &m_indexBuf);
	if (FAILED(result))
		return false;


	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
	SetDefaultTransform();
	return true;
}

void Model::ShutDownBuffers()
{
	if(m_indexBuf)
	{
		m_indexBuf->Release();
		m_indexBuf = 0;
	}
	if (m_vertexBuf)
	{
		m_vertexBuf->Release();
		m_vertexBuf = 0;
	}

	return;
}

void Model::SetDefaultTransform()
{
	m_scale = Vector3( 1, 1, 1);
	m_Rotaion = Vector3(0, 0, 0);
	m_position = Vector3(0, 0, 0);

	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* ctxt)
{
	unsigned int stride, offset;
	stride = sizeof(vertex);
	offset = 0;	
	ctxt->IASetVertexBuffers(0, 1, &m_vertexBuf, &stride, &offset);
	ctxt->IASetIndexBuffer(m_indexBuf, DXGI_FORMAT_R32_UINT,0);
	ctxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
	
}

bool Model::SerialiseMesh(string FileName, Mesh mesh)
{
	FILE* F;
	int result;
	Count c;
	c.indexCount = mesh.indexCount;
	c.vertexCount = mesh.vertexCount;
	string location = FileName;
	location[location.length()] = ' ';
	location[location.length() -1] = ' ';
	location[location.length()- 2] = ' ';
	location[location.length()- 3] = ' ';
	result = fopen_s(&F, (location + "/Verties.dat").c_str(), "w");
	if (result != 0)
		return false;
	result = fwrite(mesh.vertices, sizeof(vertex), mesh.vertexCount,F);
	if (result < mesh.vertexCount)
		return false;
	fclose(F);
	result = fopen_s(&F, (location + "/Indecies.dat").c_str(), "w");
	if (result != 0)
		return false;
	result = fwrite(mesh.indecies, sizeof(unsigned long), mesh.indexCount, F);
	if (result < mesh.indexCount)
		return false;
	fclose(F);
	result = fopen_s(&F, (location + "/Count.dat").c_str(), "w");
	if (result != 0)
		return false;
	result = fwrite(&c, sizeof(c),1, F);
	if (result < 1)
		return false;
	fclose(F);
	return true;

}

bool Model::UnSerialiseMesh(string FileName, Mesh& mesh)
{
	FILE* F;
	int result;
	Count c;
	string location = FileName;
	location[location.length()] = ' ';
	location[location.length() - 1] = ' ';
	location[location.length() - 2] = ' ';
	location[location.length() - 3] = ' ';
	result = fopen_s(&F, (location + "/Count.dat").c_str(), "r");
	if (result != 0)
		return false;
	result = fread(&c, sizeof(Count), 1, F);
	if(result < 1)
	return false;
	fclose(F);
	mesh.vertexCount = c.vertexCount;
	mesh.indexCount = c.indexCount;

	result = fopen_s(&F, (location + "/Verties.dat").c_str(), "r");
	if (result != 0)
		return false;
	result = fread(mesh.vertices, sizeof(vertex), mesh.vertexCount, F);
	if (result < mesh.vertexCount)
		return false;
	fclose(F);
	result = fopen_s(&F, (location + "/Indecies.dat").c_str(), "r");
	if (result != 0)
		return false;
	result = fread(mesh.indecies, sizeof(unsigned long), mesh.indexCount, F);
	if (result < mesh.indexCount)
		return false;
	fclose(F);
	return true;
	
}




bool Model::vertex::operator==(vertex& b)
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
