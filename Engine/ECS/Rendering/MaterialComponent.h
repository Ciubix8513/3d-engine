#pragma once
#include "MeshComponent.h"
#include <fstream>
#include <string>
#include <d3dcompiler.h>
#include "BufferStructs.h"
using namespace Engine;
using namespace EngineMath;

namespace Engine 
{
	class MaterialComponent : public Component
	{
		
		friend class Launcher;
	public:
		struct StructBuffer
		{
			BufferClass* Buffer;
			const std::type_info* type;
		};
		union ShaderBufferType
		{			
			float Float;
			Vector2 Vector2;
			Vector3 Vector3;
			Vector4 Vector4;
			Matrix4x4 Matrix;
			StructBuffer Buffer;
			ShaderBufferType();

		};
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
			ID3D11Buffer* Buffer;
			size_t bufferNum;
			ShaderType type;
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth);
			bool CreateBuffer(ID3D11Device* device, size_t ByteWidth,D3D11_SUBRESOURCE_DATA* InitialData);
		};
		struct BufferBuffer
		{
			std::string name;
			ShaderBufferType data;
			DataType type;
			BufferBuffer(std::string name);
		};
		
	public:
		//List of predefined buffers
		/*struct MatrixBuffer
		{
			Matrix4x4
				worldMatrix,
				viewMatrix,
				projectionMatrix,
				objectMat;
		};
		struct CameraBuffer
		{
			Vector3 postion;
			float padding;
		};*/
#pragma endregion
#pragma region Set shader params functions
		void SetFloat( std::string name, float data);
		void SetVector2(std::string name, Vector2 data);
		void SerVector3(std::string name, Vector3 data);
		void SetVector4(std::string name, Vector4 data);
		void SetMatrix(std::string name, Matrix4x4 data);
		void SetSampler(std::string name, ID3D11SamplerState* data); //TODO: add sampler
		//bool SetTexture(std::string name, float data);//TODO: add texture		
		void SetStruct(std::string name, BufferClass* data,const std::type_info* bufferType);

#pragma endregion
	public:
		std::vector<const type_info*> GetRequieredComponents() override;
		void Initialise(std::vector<Component**> Comps) override;
		void Shutdown() override;
		size_t GetRenderingOrder();
		void SetRenderingOrder(size_t NewRenderingOrder);

	private:
		bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, std::string ShaderName, UINT FLAGS1 = D3D10_SHADER_ENABLE_STRICTNESS,UINT FLAGS2 = 0);
		std::string GetShaderErrorMsg(ID3D10Blob* msg);
		std::vector<std::string> GetWordsFromFile(WCHAR* fileName);
		bool PreProcessShader(WCHAR* fileName);
		bool Render(ID3D11DeviceContext* ctxt, int IndexCount);
	private: 
		size_t RenderingOrder;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		std::vector<Buffer> m_buffers;
		std::vector<BufferBuffer> m_buffersBuffer;
		std::vector<Sampler> m_samplerBuffer;

		MeshComponent** mesh; //To get the data
	};
}
