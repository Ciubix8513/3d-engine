#pragma once
#include "MeshComponent.h"
#include <fstream>
#include <string>
#include <d3dcompiler.h>
using namespace Engine;
using namespace EngineMath;

namespace Engine 
{
	class MaterialComponent : public Component
	{
		//ShaderBuffer union declaration
		friend class Launcher;
	public:
		union ShaderBufferType
		{
			bool Bool;
			float Float;
			Vector2 Vector2;
			Vector3 Vector3;
			Vector4 Vector4;
			Matrix4x4 Matrix;
		};
#pragma region Structs

		struct Buffer
		{
			ID3D11Buffer* Buffer;
			std::string BufferName;
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth);
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth,D3D11_SUBRESOURCE_DATA* InitialData);
		};


		//List of predefined buffers

		struct MatrixBuffer
		{
			Matrix4x4
				worldMatrix,
				viewMatrix,
				projectionMatrix,
				objectMat;
		};
#pragma endregion
#pragma region Set shader params functions
		bool SetFloat(std::string name, float data);
		bool SetVector2(std::string name, Vector2 data);
		bool SerVector3(std::string name, Vector3 data);
		bool SetVector4(std::string name, Vector4 data);
		bool SetMatrix(std::string name, Matrix4x4 data);
		bool SetSampler(std::string name, ID3D11SamplerState data); //TODO: add sampler
		bool SetTexture(std::string name, float data);//TODO: add texture
		
		bool SetStruct();
#pragma endregion





		std::vector<const type_info*> GetRequieredComponents() override;
	public:
		size_t GetRenderingOrder();
		void SetRenderingOrder(size_t NewRenderingOrder);
		 static bool PreProcessShader(WCHAR* fileName);



	private:
		bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, std::string ShaderName, UINT FLAGS1 = D3D10_SHADER_ENABLE_STRICTNESS,UINT FLAGS2 = 0);
		std::string GetShaderErrorMsg(ID3D10Blob* msg);
		static std::vector<std::string> GetWordsFromFile(WCHAR* fileName);

	private: 
		size_t RenderingOrder;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
	};
}
