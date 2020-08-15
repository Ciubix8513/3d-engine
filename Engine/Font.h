#pragma once
#ifndef _FONT_H_
#define _FONT_H_

#pragma region includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
using namespace std;
using namespace DirectX;
#include "Texture.h"
#pragma endregion



class Font
{
#pragma region structss
private:
	struct FontType
	{
		double left, right;
		int size;
	};
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 Texture;
	};
#pragma endregion
#pragma region Public funcs
public:
	Font();
	Font(const Font&);
	~Font();
	bool Init(ID3D11Device*,ID3D11DeviceContext*, char*, char*,int);
	void ShutDown();
	ID3D11ShaderResourceView* GetTexture();
	void BuidVertexarray(void*,char*,float,float);
#pragma endregion
#pragma region Private funcs
	bool LoadFontData(char* file, int numChars);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
#pragma endregion
#pragma region Private vars
private:
	FontType* m_Font;
	Texture* m_Texture;
#pragma endregion


};
#endif 
