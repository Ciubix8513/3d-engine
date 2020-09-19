#pragma once
#ifndef _FONTSHADER_H_
#define _FONTSHADER_H_
#pragma region includes
#include <d3d11.h>
#include "../System/Math/EngineMath.h"
#include <d3dcompiler.h>
#include <fstream>
using namespace EngineMath;
using namespace std;
#pragma endregion


class FontShader
{
private:
	struct MatrixBuffer
	{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
	};
	struct Pixelbuffer
	{
		Vector4 pixelColor;
	};


#pragma region public funcs
public:
	FontShader();
	~FontShader();
	FontShader(const FontShader&);
	void ShutDown();
	bool Init(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext*, int, Matrix4x4, Matrix4x4, Matrix4x4,ID3D11ShaderResourceView*, Vector4 pixelColor);
#pragma endregion
#pragma region  private Funcs
private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParams(ID3D11DeviceContext*, Matrix4x4, Matrix4x4, Matrix4x4, ID3D11ShaderResourceView*, Vector4 pixelColor);
	void RenderShader(ID3D11DeviceContext*, int);
#pragma endregion
#pragma region private Vars
private:
	ID3D11SamplerState* m_SS;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matBuffer;
	ID3D11Buffer* m_pixelBuffer;
#pragma endregion

};

#endif