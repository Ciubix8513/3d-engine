#pragma once
#pragma region TechStuff
#ifndef _MODEL_H_
#define _MODEL_H_
#pragma endregion
#pragma region includes
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace DirectX;
using namespace std;
#pragma endregion


	class Model
	{
	public:

		
		struct vertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
			XMFLOAT2 UV;
			XMFLOAT3 normal;
		
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
		XMMATRIX Getobjectmatrix();
		Mesh loadMeshFromFile(char*);
		void Shutdown();
		bool Init(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
		void SetRotation(float, float, float);
		void SetRotation(XMVECTOR);
		void Render(ID3D11DeviceContext*, ID3D11Device*);
		XMFLOAT3 GetRotation();
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
#pragma endregion
#pragma region Private vars
	private:
		ID3D11Buffer* m_vertexBuf, * m_indexBuf;
		int m_vertexCount, m_indexCount;
		Texture* m_texture;
		XMFLOAT3 m_Rotaion;
		XMFLOAT3 m_scale;
		XMFLOAT3 m_position;
		XMFLOAT3 m_RotationOrigin;
#pragma endregion
	};

#endif