#pragma once
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_
#pragma region includes
#include <d3d11.h>
#include "../System/Math/EngineMath.h"
#include <d3dcompiler.h>
#include <fstream>
using namespace EngineMath;
using namespace std;
#pragma endregion


class TextureShader
{
private:
	struct MatrixBuffer
	{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
	
	};

#pragma region public funcs
public:
	TextureShader();
	~TextureShader();
	TextureShader(const TextureShader&);
	void ShutDown();
	bool Init(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* ctxt, int indexCount, Matrix4x4 world, Matrix4x4 view, Matrix4x4 project, ID3D11ShaderResourceView* texture);
#pragma endregion
#pragma region  private Funcs
private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParams(ID3D11DeviceContext* ctxt, Matrix4x4 world, Matrix4x4 view, Matrix4x4 proj, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext*, int);
#pragma endregion
#pragma region private Vars
private:
	ID3D11SamplerState* m_SS;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matBuffer;
#pragma endregion

};

#endif