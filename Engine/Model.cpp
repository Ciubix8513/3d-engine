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


vector<string> ListToVector(list<string> List) {
	vector<string> Vector(List.begin(), List.end());
	return Vector;
}

XMMATRIX Model::Getobjectmatrix()
{
	XMMATRIX output; 
	output = XMMatrixAffineTransformation(XMLoadFloat3(&m_scale), XMLoadFloat3(&m_RotationOrigin), XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotaion)), XMLoadFloat3(&m_position));

	return output;
}

Model::Mesh Model::loadMeshFromFile(char* fileName)
{
	Mesh mesh = Mesh();
	ifstream file(fileName);
	string line;
	string fstChar = "01";
	list<string> vLines,vtLines,vnLines,fLines;
	vector<string> wipVec;
	if (!file.is_open())
		return Mesh();
	int vCount = 0 , vtCount = 0, vnCount = 0, fCount = 0;
	while (getline(file,line))
	{
		fstChar[0] = line[0];
		fstChar[1] = line[1];
		if(fstChar == "v ")
		{
			vLines.push_back(line);
			vCount++;
		}
		else if(fstChar == "vt")
		{
			vtLines.push_back(line);

			vtCount++;
		}else if(fstChar == "vn")
		{
			vnLines.push_back(line);
			vnCount++;
		}
		else if(fstChar == "f ")
		{
			fLines.push_back(line);
			fCount++;
		}
	}
	file.close();

	XMFLOAT3* ver = new XMFLOAT3[vCount];
	XMFLOAT2* Tex = new XMFLOAT2[vtCount];
	XMFLOAT3* Norm = new XMFLOAT3[vnCount];
	unsigned long* ind = new unsigned long[fCount*3];
	string procesing ;
	#pragma region  Vertices
	wipVec = ListToVector(vLines);
	for (int i = 0; i < vCount; i++)
	{
		procesing = wipVec[i];
		int* begining = new int[3]; int* end = new int[3];
		bool start = false;
		int counter = 0;
		begining[0] = 2;
		end[2] = procesing.size();
		for (int j = 2; j < procesing.size(); j++)
		{

			if (procesing[j] == ' ')
			{
				end[counter] = j;
				counter++;
				begining[counter] = j + 1;

			}
		}
		string* value = new string[3];
		for (int j = 0; j < 3; j++)
		{
			for (int k = begining[j]; k < end[j]; k++)
			{
				value[j] += procesing[k];
			}
		}
		XMFLOAT3 a = XMFLOAT3(atof(value[0].c_str()), atof(value[1].c_str()), atof(value[2].c_str()));
		ver[i] = a;
	}
	#pragma endregion
	#pragma region UVs
	wipVec = ListToVector(vtLines);
	for (int i = 0; i < vtCount; i++)
	{
		procesing = wipVec[i];
		int* begining = new int[2]; int* end = new int[2];
		bool start = false;
		int counter = 0;
		begining[0] = 3;
		end[2] = procesing.size();
		for (int j = 3; j < procesing.size(); j++)
		{

			if (procesing[j] == ' ')
			{
				end[counter] = j;
				counter++;
				begining[counter] = j + 1;

			}
		}
		string* value = new string[2];
		for (int j = 0; j < 2; j++)
		{
			for (int k = begining[j]; k < end[j]; k++)
			{
				value[j] += procesing[k];
			}
		}
		XMFLOAT2 a = XMFLOAT2(atof(value[0].c_str()), atof(value[1].c_str()));
		Tex[i] = a;
	}
	#pragma endregion
	#pragma region Normals
	wipVec = ListToVector(vnLines);
	for (int i = 0; i < vnCount; i++)
	{
		procesing = wipVec[i];
		int* begining = new int[3]; int* end = new int[3];
		bool start = false;
		int counter = 0;
		begining[0] = 3;
		end[2] = procesing.size();
		for (int j = 3; j < procesing.size(); j++)
		{

			if (procesing[j] == ' ')
			{
				end[counter] = j;
				counter++;
				begining[counter] = j + 1;

			}
		}
		string* value = new string[3];
		for (int j = 0; j < 3; j++)
		{
			for (int k = begining[j]; k < end[j]; k++)
			{
				value[j] += procesing[k];
			}
		}
		XMFLOAT3 a = XMFLOAT3(atof(value[0].c_str()), atof(value[1].c_str()), atof(value[2].c_str()));
		Norm[i] = a;
	}
	#pragma endregion
	cout << "vertices" << endl;
	for (size_t i = 0; i < vCount; i++)
	{
		cout << ver[i].x << " " << ver[i].y << " " << ver[i].z << endl;
	}

	#pragma region tris
	wipVec = ListToVector(fLines);
	vertex *vs = new vertex[fCount * 3];
	list<int> missing;
	for (int i = 0; i < fCount; i++)
	{		
		procesing = wipVec[i];

		int counter = 0;	
		int Begining[3][ 3];
		int End[3][ 3];


		Begining[0][ 0] = 2;
		End[2][ 2] = procesing.size();
		int Xcounter = 0, Ycounter = 0;
		for (int j = 2; j < procesing.size(); j++)
		{
			if (procesing[j] == ' ')
			{
				End[Xcounter][ Ycounter] = j;
				Ycounter++;
				Xcounter = 0;
				Begining[Xcounter][ Ycounter] = j + 1;
			}
			else if (procesing[j] == '/') 
			{
				End[Xcounter][ Ycounter] = j;
				Xcounter++;
				Begining[Xcounter][Ycounter] = j + 1;
			}
		}
		string value;
		int indecies [3][3];
		//creating index array
		for (int j = 0; j < 3; j++)
		{
			for (int  k = 0; k < 3; k++)
			{
				for (int w = Begining[k][j]; w < End[k][j]; w++)
				{
					value += procesing[w];

				}
				cout << 1;

				indecies[j][k] = atof(value.c_str()) -1;
				value = "";
			}
		}

		vertex v[3];
		//creating vertices
		for (int j = 0; j < 3; j++)
		{
			v[j].position = ver[indecies[j][ 0]];
			v[j].UV = Tex[indecies[j][ 1]];
			v[j].normal = Norm[indecies[j][ 2]];
			bool exists = false;
			int existingIndex;

			for (int k = 0; k < fCount*3; k++)
			{
				if (vs[k] == v[k])
				{
					exists = true;
					existingIndex = k;
				}
			}
			if (!exists)
				vs[(i * 3) + j] = v[j];
			else
			{
				missing.push_back(existingIndex);
			}
				
			
		}
		cout << "VS = " << endl;
		for (int i = 0; i < fCount * 3; i++)
		{
			cout << vs[i].position.x << " " << vs[i].position.y << " " << vs[i].position.z << endl;
		}

	}
	#pragma endregion
	vertex* t = new vertex[1];
	int  missingCounter = 0;
	vector<int> m(missing.begin(),missing.end());
	for (int i = 0; i < fCount*3; i++)
	{
		if (vs[i] == t[0]) 
		{
			ind[i] = m[missingCounter];
			missingCounter ++ ;
		}
		else
		{
			ind[i] = i;
		}
	}	
	
	mesh.indecies = new unsigned long[fCount *3];
	mesh.vertices = new vertex[fCount*3];
	for (size_t i = 0; i < fCount *3; i++)
	{
		mesh.vertices[i] = vs[i];
		mesh.indecies[i] = ind[i];
	}
	mesh.vertexCount = fCount * 3;
	
	mesh.indexCount = fCount * 3;
	
	return mesh;
}





bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename,char* file)
{
	bool result;
	result = InitBuffers(device,file);
	if (!result)
		return false;
	result = LoadTexture(device, deviceContext, textureFilename);\
	if (!result)
			return false;
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
	m_Rotaion = XMFLOAT3(x, y, z);

}

void Model::SetRotation(XMVECTOR v)
{
	XMFLOAT3 vec;
	XMStoreFloat3(&vec, v);
x:
	if (vec.x >= 360)
	{
		vec.x -= 360;
		goto x;
	}
y:
	if (vec.y >= 360)
	{
		vec.y -= 360;
		goto y;
	}
z:
	if (vec.z >= 360)
	{
		vec.z -= 360;
		goto z;
	}
	m_Rotaion =vec;

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

XMFLOAT3 Model::GetRotation()
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
		vertices[0].color = XMFLOAT4(1, 1, 1, 1);
		vertices[0].position = XMFLOAT3(-1, -1, 0);
		vertices[0].UV = XMFLOAT2(0.0f, .7f);
		vertices[0].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

		vertices[1].color = XMFLOAT4(1, 1, 1, 1);
		vertices[1].position = XMFLOAT3(0, 1, 0);
		vertices[1].UV = XMFLOAT2(0.5f, 0.0f);
		vertices[1].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

		vertices[2].color = XMFLOAT4(1, 1, 1, 1);
		vertices[2].position = XMFLOAT3(1, -1, 0);
		vertices[2].UV = XMFLOAT2(1.0f, 0.7f);
		vertices[2].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
	}
	else {
		Mesh* mesh = &loadMeshFromFile(file);
		if (mesh == &Mesh())
			goto Default;
		m_indexCount = mesh->indexCount;
		m_vertexCount = mesh->vertexCount;
		vertices = mesh->vertices;
		indices = mesh->indecies;
	}


	vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.ByteWidth = sizeof(vertex) * m_vertexCount;
	vertexBuffDesc.BindFlags = 0x1L;//0x1L = bind vertex buffer
	vertexBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
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
	m_scale = XMFLOAT3( 1, 1, 1);
	m_Rotaion = XMFLOAT3(0, 0, 0);
	m_position = XMFLOAT3(0, 0, 2);	
	m_RotationOrigin = XMFLOAT3(0, 0, 0); 
	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* ctxt)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(vertex);
	offset = 0;


	
	ctxt->IASetVertexBuffers(0, 1, &m_vertexBuf, &stride, &offset);
	ctxt->IASetIndexBuffer(m_indexBuf, DXGI_FORMAT_R32_UINT,0);
	ctxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
	
}

bool Model::vertex::operator==(vertex& b)
{
	bool e = false;
	vertex* a = this;
	e *= a->color.x == b.color.x;
	e *= a->color.y == b.color.y;
	e *= a->color.z == b.color.z;
	e *= a->color.w == b.color.w;
	e *= a->normal.x == b.normal.x;
	e *= a->normal.z == b.normal.z;
	e *= a->normal.y == b.normal.y;
	e *= a->position.x == b.position.x;
	e *= a->position.y == b.position.y;
	e *= a->position.z == b.position.z;
	e *= a->UV.x == b.UV.x;
	e *= a->UV.y == b.UV.y;
	return e;
}
