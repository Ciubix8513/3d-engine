#pragma once
#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_
#pragma region includes
#include <d3d11.h>
#include "../System/Math/EngineMath.h"
#include <d3dcompiler.h>
#include <fstream>
using namespace EngineMath;
using namespace std;
#pragma endregion


class LightShader
{
private:
	struct MatrixBuffer
	{
		Matrix4x4 world;
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 object;
	};
	struct CameraBuffer
	{
		Vector3 cameraPos;
		float padding;
	};
	struct LightBuffer
	{
		Vector4 AmbientColor;
		Vector4 diffuseColor;
		Vector3 lightDirection;
		float specularPower;
		Vector4 specularColor;
	};

#pragma region public funcs
public:
	LightShader();
	~LightShader();
	LightShader(const LightShader&);
	void ShutDown();
	bool Init(ID3D11Device* device, HWND hwnd);
	bool Render(ID3D11DeviceContext* ctxt, int indexCount, Matrix4x4 world, Matrix4x4 view, Matrix4x4 project, Matrix4x4 object, ID3D11ShaderResourceView* texture, Vector3 lightDir, Vector4 diffuseColor, Vector4 ambientColor, Vector4 specularColor, float specularPow, Vector3 cameraPos);
#pragma endregion
#pragma region  private Funcs
private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParams(ID3D11DeviceContext*, Matrix4x4, Matrix4x4, Matrix4x4, Matrix4x4, ID3D11ShaderResourceView*, Vector3, Vector4, Vector4, Vector4,float, Vector3);
	void RenderShader(ID3D11DeviceContext*, int);
#pragma endregion
#pragma region private Vars
private:
	ID3D11SamplerState* m_SS;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
#pragma endregion

};

#endif