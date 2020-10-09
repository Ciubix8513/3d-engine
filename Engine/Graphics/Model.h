#pragma once
#pragma region TechStuff
#ifndef _MODEL_H_
#define _MODEL_H_
#pragma endregion
#pragma region includes
#include <d3d11.h>
#include "../System/Math/EngineMath.h"
#include "Texture.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace EngineMath;
using namespace std;
#pragma endregion


	class Model
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
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 UV;
			DirectX::XMFLOAT3 normal;
		
			bool operator==(vertex& other);
			
		};
		struct Mesh
		{
			vertex* vertices;
			unsigned long* indecies;
			int vertexCount;
			int indexCount;
		};
#pragma region Public funcs
	public:
		Model();
		Model(Model&);
		~Model();
		Matrix4x4 Getobjectmatrix();
		Mesh loadMeshFromFile(char*);
		void Shutdown();
		bool Init(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
		void SetRotation(float, float, float);
		void SetRotation(Vector3);
		void Render(ID3D11DeviceContext*, ID3D11Device*);
		Vector3 GetRotation();
		int GetIndexCount();
		ID3D11ShaderResourceView* GetTexture();
#pragma endregion
#pragma region Private funcs
	private:
		bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
		bool InitBuffers(ID3D11Device*, char*);
		void ShutDownBuffers();
		void SetDefaultTransform();
		void RenderBuffers(ID3D11DeviceContext*);
		bool SerialiseMesh(string fileLocation, Mesh mesh);
		bool UnSerialiseMesh(string fileLocation , Mesh& mesh);
#pragma endregion
#pragma region Private vars
	private:
		
		ID3D11Buffer* m_vertexBuf, * m_indexBuf;
		int m_vertexCount, m_indexCount;
		Texture* m_texture;
		Vector3 m_Rotaion;
		Vector3 m_scale;
		Vector3 m_position;

#pragma endregion
	};

#endif