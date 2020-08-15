#pragma once
#ifndef _BITMAP_H_
#define _BITMAP_H_
#pragma region includes
#include <d3d11.h>
#include <iostream>
#include <DirectXMath.h>
using namespace DirectX;
#include "Texture.h"
#pragma endregion



class Bitmap
{
#pragma region structs
	struct BitmapVertex
	{
		XMFLOAT3 position;
		XMFLOAT2 Texture;
	};
#pragma endregion	
#pragma region PublicFuncs
public:
	Bitmap();
	Bitmap(const Bitmap&);
	~Bitmap();
	bool Init(ID3D11Device*, int, int, char*, int, int);
	void ShutDown();
	bool Render(ID3D11DeviceContext*,int,int);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
#pragma endregion
#pragma region PrivateFuncs
private:
	bool Initbuffers(ID3D11Device*);
	void ShutDownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*,int,int);
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, char*);
	void Releasetexture();
#pragma endregion
#pragma region PrivateVars
private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	int m_screenWidth, m_screenHeight,
		m_bimapWidth, m_bitmapHeight,
		m_PreviosPosX, m_PreviosPosY;
#pragma endregion
};
#endif

