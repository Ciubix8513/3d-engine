#include "MeshComponent.h"

void Engine::MeshComponent::Initialise()
{
	
}

Engine::MeshComponent::MeshComponent()
{


}



bool Engine::MeshComponent::loadMeshFromFile(string fileName)
{
	string path = (fileName + ".EngnMdl");

	if(FILE* file = fopen((path + ".data").c_str(),"r"))
	{
		fclose(file);
		m_Model = DeSerialiseMesh(path);
		return true;
	}

	Mesh mesh = Mesh();
	ifstream file(fileName);
	string line;
	string fstChar = "01";

	vector<string> wipVec;
	if (!file.is_open())
	{
		cout << "Failed to open file";
		throw exception("Failed to open the file");
		return false;
	}
	cout << "opened file" << endl;
	int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0;
	
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

	mesh.indexCount = Indecies.size();
	mesh.vertexCount = FinalVertices.size();
	mesh.indecies = new unsigned long[Indecies.size()];
	mesh.vertices = new vertex[FinalVertices.size()];
	for (size_t i = 0; i < mesh.vertexCount; i++)
		mesh.vertices[i] = FinalVertices[i];
	for (size_t i = 0; i < mesh.indexCount; i++)
		mesh.indecies[i] = Indecies[i];

	m_Model = mesh;
	SerialiseMesh(path);
	return true;
}

void Engine::MeshComponent::SerialiseMesh(string path)
{	
	FILE *file;
	string Path = path + ".data";
	Count count;
	count.indexCount = m_Model.indexCount;
	count.vertexCount = m_Model.vertexCount;

	file = fopen(Path.c_str(), "w");
	fwrite(&count, sizeof(count), 1, file);
	fclose(file);

	Path = path + ".vert";
	file = fopen(Path.c_str(), "w");
	fwrite(m_Model.vertices, sizeof(vertex), m_Model.vertexCount, file);
	fclose(file);

	Path = path + ".ind";
	file = fopen(Path.c_str(), "w");
	fwrite(m_Model.indecies, sizeof(unsigned long), m_Model.indexCount, file);
	fclose(file);

	return;
}

Engine::MeshComponent::Mesh Engine::MeshComponent::DeSerialiseMesh(string path)
{
	Mesh mesh;
	FILE* file;
	string Path = path + ".data";
	Count count;
	
	file = fopen(Path.c_str(), "r");
	fread(&count, sizeof(count), 1, file);	
	fclose(file);
	
	mesh.indexCount = count.indexCount;
	mesh.vertexCount = count.vertexCount;

	mesh.vertices = new vertex[count.vertexCount];
	mesh.indecies = new unsigned long[count.indexCount];

	Path = path + ".vert";
	file = fopen(Path.c_str(), "r");
	fread(mesh.vertices, sizeof(vertex), count.vertexCount, file);
	fclose(file);

	Path = path + ".ind";
	file = fopen(Path.c_str(), "r");
	fread(mesh.indecies, sizeof(unsigned long), count.indexCount, file);
	fclose(file);


	return mesh;
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

