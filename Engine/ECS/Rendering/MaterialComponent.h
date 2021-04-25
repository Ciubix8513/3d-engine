#pragma once
#include "MeshComponent.h"
#include <fstream>
#include <string>
#include <d3dcompiler.h>
#include <atlbase.h>
#include <atlconv.h>
#include "BufferStructs.h"
using namespace Engine;
using namespace EngineMath;

namespace Engine 
{
	class MaterialComponent : public Component
	{
		
		friend class Launcher;
#pragma region Structs
		
	private:
		enum ShaderType
		{
			VertexShader, PixelShader
		};
		enum DataType
		{
			Float, vector2, vector3, vector4, Matrix,Struct
		};		
		struct Sampler
		{
			ID3D11SamplerState* sampler;
			std::string name;
			ShaderType type;
			size_t samplerNum;
			bool CreateSampler(ID3D11Device* device);
		};
		struct Buffer
		{
			std::string name;
			ID3D11Buffer* Buffer;
			size_t bufferNum;
			ShaderType type;
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth,std::string name);
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth,D3D11_SUBRESOURCE_DATA* InitialData, std::string name);
		};
#pragma endregion
#pragma region Set shader params functions
	public:
		bool SetFloat( std::string name, float data);
		bool SetVector2(std::string name, Vector2 data);
		bool SerVector3(std::string name, Vector3 data);
		bool SetVector4(std::string name, Vector4 data);
		bool SetMatrix(std::string name, Matrix4x4 data);
		bool SetSampler(std::string name, ID3D11SamplerState* data); 
		//bool SetTexture(std::string name, float data);//TODO: add texture		
		
		template<typename T>	
		bool SetStruct(std::string Name, T Data)
		{
			for (Buffer B : m_buffers)
				if (B.name == Name)
				{
					HRESULT res;
					D3D11_MAPPED_SUBRESOURCE data;
					res = (*m_D3dPtr)->getDeviceContext()->Map(B.Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
					if (FAILED(res))
						return false;
					*(T*)data.pData = Data;
					(*m_D3dPtr)->getDeviceContext()->Unmap(B.Buffer, 0);
					return true;
				}

			return false;
		};

#pragma endregion
	public:
		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component*> Comps, D3d** d3d) override;
		void Shutdown() override;
		size_t GetRenderingOrder();
		void SetRenderingOrder(size_t NewRenderingOrder);
		void Render();
		bool InitShader(std::string vsFilename, std::string psFilename, std::string ShaderName, UINT FLAGS1 = D3D10_SHADER_ENABLE_STRICTNESS, UINT FLAGS2 = 0);

	private:
		std::string GetShaderErrorMsg(ID3D10Blob* msg);
		std::vector<std::string> GetWordsFromFile(std::string fileName);
		bool PreProcessShader(std::string fileName);

	private: 
		size_t RenderingOrder;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		std::vector<Engine::MaterialComponent::Buffer> m_buffers;
		std::vector<Engine::MaterialComponent::Sampler> m_samplerBuffer;
		int m_VSBnum, m_PSBnum;
		
		MeshComponent* m_mesh; //To get the data
	};
}
