#pragma once
#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_
#pragma region includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace DirectX;
using namespace std;
#pragma endregion


class LightShader
{
private:
	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX object;
	};
	struct LightBuffer
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

#pragma region public funcs
public:
	LightShader();
	~LightShader();
	LightShader(const LightShader&);
	void ShutDown();
	bool Init(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3 , XMFLOAT4 );
#pragma endregion
#pragma region  private Funcs
private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParams(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3 , XMFLOAT4 );
	void RenderShader(ID3D11DeviceContext*, int);
#pragma endregion
#pragma region private Vars
private:
	ID3D11SamplerState* m_SS;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matBuffer;
	ID3D11Buffer* m_lightBuffer;
#pragma endregion

};

#endif