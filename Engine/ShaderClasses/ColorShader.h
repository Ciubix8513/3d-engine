#pragma once
#pragma region TechStuff
#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_
#pragma endregion
#pragma region  Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "../System/Math/EngineMath.h"
using namespace EngineMath;
using namespace std;
#pragma endregion
class ColorShader
{
private:
	struct MatrixBuffer
	{
		 Matrix4x4 world;
		 Matrix4x4 view;
		 Matrix4x4 projection;
	};
#pragma region  public funcs
public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();
	bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, Matrix4x4, Matrix4x4, Matrix4x4);
#pragma endregion
#pragma region  private Funcs
private :
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);
	bool SetShaderParams(ID3D11DeviceContext*, Matrix4x4, Matrix4x4, Matrix4x4);
	void RenderShader(ID3D11DeviceContext*, int);
#pragma endregion
#pragma region  private vars
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matBuffer;
#pragma endregion



};

#endif