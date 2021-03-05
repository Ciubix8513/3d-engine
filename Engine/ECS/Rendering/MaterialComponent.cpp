#include "MaterialComponent.h"

std::vector<const type_info*> Engine::MaterialComponent::GetRequieredComponents()
{
	return {&typeid(Engine::MeshComponent)};
}

size_t Engine::MaterialComponent::GetRenderingOrder()
{
	return RenderingOrder;
}

void Engine::MaterialComponent::SetRenderingOrder(size_t NewRenderingOrder)
{
	RenderingOrder = NewRenderingOrder;
	return;
}

bool Engine::MaterialComponent::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, std::string ShaderName, UINT FLAGS1 = D3D10_SHADER_ENABLE_STRICTNESS, UINT FLAGS2)
{
	HRESULT result;
	ID3D10Blob* errorMsg;
	ID3D10Blob* vertexshaderBuff;
	ID3D10Blob* pixelShaderBuff;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matBuffDesc, lighBuffDesc, cameraBuffDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	vertexshaderBuff = 0;
	pixelShaderBuff = 0;

	//Compiling vertex shader
	result = D3DCompileFromFile(vsFilename, NULL, NULL,(( ShaderName + "VertexShader").c_str()), "vs_5_0", FLAGS1, FLAGS2, &vertexshaderBuff, &errorMsg);
	if(FAILED(result))
	{
		if (errorMsg)
			throw exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw exception(((char*)vsFilename + (string)" does not exist").c_str());
		return false;
	}
	//Compiling pixel shader
	result = D3DCompileFromFile(psFilename, NULL, NULL, ((ShaderName + "PixelShader").c_str()), "ps_5_0", FLAGS1, FLAGS2, &pixelShaderBuff, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			throw exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw exception(((char*)psFilename + (string)" does not exist").c_str());
		return false;
	}

	//Creating vertex shader
	result = device->CreateVertexShader(vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		throw exception("Could not create vertex shader");
		return false;
	}

	//Creating pixel shader
	result = device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		throw exception("Could not create pixel shader");
		return false;
	}
	
	//Creating poligon layout
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Creating Input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		throw exception("Failed to create input layout");
		return false;
	}

	//Releasing buffers
	vertexshaderBuff->Release();
	pixelShaderBuff->Release();
	vertexshaderBuff = 0;
	pixelShaderBuff = 0;

	return true;
}

std::string Engine::MaterialComponent::GetShaderErrorMsg(ID3D10Blob* msg)
{
	string compileErrors;
	compileErrors = (char*)(msg->GetBufferPointer());
	msg->Release();
	msg = 0;
	return compileErrors;
}
