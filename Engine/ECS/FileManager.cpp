#include "FileManager.h"



MeshComponent::Mesh* Engine::FileManager::LoadMesh(std::string FileName)
{
	MeshComponent::Mesh mesh = MeshComponent::Mesh();
	std::ifstream file(FileName);
	std::string line;
	std::string fstChar = "01";

	std::vector<std::string> wipVec;
	if (!file.is_open())
	{
		std::cout << "Failed to open file";
		return nullptr;
	}
	std::cout << "opened file" << std::endl;
	int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0;

	//std::vector<Vector2> a;


	std::vector<Vector3> Vertices, Normals;
	std::vector<MeshComponent::vertex> Tris;
	std::vector<Vector2> UVs;
	while (getline(file, line))
	{

		if (line[0] == 'v' && line[1] == ' ')
		{

			std::string currentFloat;
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

			std::string currentFloat;
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

			std::string currentFloat;
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

			MeshComponent::vertex currentVertex;
			std::string curentIndex;
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
	std::vector<MeshComponent::vertex> FinalVertices;
	std::vector<unsigned long> Indecies;
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
		std::cout << '\n';
	}
	Vertices.clear();
	Normals.clear();
	Tris.clear();
	UVs.clear();

	/*
	auto SetVertices = FinalVertices;
	auto Setindecies = Indecies;

	mesh.indexCount = Setindecies.size();
	mesh.MeshComponent::vertexCount = SetVertices.size();
	mesh.indecies = new unsigned long [Setindecies.size()];
	mesh.vertices = new MeshComponent::vertex[SetVertices.size()];
	for (size_t i = 0; i < mesh.MeshComponent::vertexCount; i++)
		mesh.vertices[i] = SetVertices[i];
	for (size_t i = 0; i < mesh.indexCount; i++)
		mesh.indecies[i] = Setindecies[i];
	*/

	mesh.indexCount = Indecies.size();
	mesh.vertexCount = FinalVertices.size();
	mesh.indecies = new unsigned long[Indecies.size()];
	mesh.vertices = new MeshComponent::vertex[FinalVertices.size()];
	for (size_t i = 0; i < mesh.vertexCount; i++)
		mesh.vertices[i] = FinalVertices[i];
	for (size_t i = 0; i < mesh.indexCount; i++)
		mesh.indecies[i] = Indecies[i];

	return &mesh;
}
