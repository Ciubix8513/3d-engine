#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#pragma region includes
#include <d3d11.h>
#include <stdio.h>
#pragma endregion
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
#pragma region  Public funcs
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
#pragma endregion
#pragma region private funcs
private:
	bool loadTarga(char*, int&, int&);
#pragma endregion
#pragma region private vars
private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
#pragma endregion
};

#endif