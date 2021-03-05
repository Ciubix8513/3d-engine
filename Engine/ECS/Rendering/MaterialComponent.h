#pragma once
#include "MeshComponent.h"
#include <fstream>
#include <d3dcompiler.h>
using namespace Engine;
using namespace EngineMath;

namespace Engine 
{
	class MaterialComponent : public Component
	{
		std::vector<const type_info*> GetRequieredComponents() override;
	public:
		size_t GetRenderingOrder();
		void SetRenderingOrder(size_t NewRenderingOrder);

	private:
		bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, std::string ShaderName, UINT FLAGS1 = D3D10_SHADER_ENABLE_STRICTNESS,UINT FLAGS2 = 0);
		std::string GetShaderErrorMsg(ID3D10Blob* msg);
	private: 
		size_t RenderingOrder;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
	};
}
