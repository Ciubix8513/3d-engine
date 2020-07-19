#include "TextureShader.h"

TextureShader::TextureShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matBuffer = 0;
	m_SS = 0;
}

TextureShader::~TextureShader()
{
}
TextureShader::TextureShader(const TextureShader&)
{
}
void TextureShader::ShutDown()
{
	ShutdownShader();
	return;
}

bool TextureShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitShader(device, hwnd, (WCHAR*)L"../Engine/Shaders/TextureVS.hlsl", (WCHAR*)L"../Engine/Shaders/Texture.hlsl");
	return result;
}

bool TextureShader::Render(ID3D11DeviceContext* ctxt, int indexCount, XMMATRIX world, XMMATRIX view, XMMATRIX project,XMMATRIX object, ID3D11ShaderResourceView* texture)
{
	bool result;
	result = SetShaderParams(ctxt, world, view, project,object, texture);
	if (!result)
		return false;
	RenderShader(ctxt, indexCount);

	return true;
}

bool TextureShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMsg;
	ID3D10Blob* vertexshaderBuff;
	ID3D10Blob* pixelShaderBuff;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matBuffDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMsg = 0;
	vertexshaderBuff = 0;
	pixelShaderBuff = 0;

	result = D3DCompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexshaderBuff, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		return false;
	}
	result = D3DCompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuff, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, psFilename);
		else
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		return false;
	}

	result = device->CreateVertexShader(vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not create vertex shader.", L"Error", 0x00000000L);
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not create pixel shader.", L"Error", 0x00000000L);
		return false;
	}
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

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	result = device->CreateInputLayout(polygonLayout, numElements, vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;
	vertexshaderBuff->Release();
	vertexshaderBuff = 0;
	pixelShaderBuff->Release();
	pixelShaderBuff = 0;

	matBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	matBuffDesc.ByteWidth = sizeof(MatrixBuffer);
	matBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matBuffDesc.MiscFlags = 0;
	matBuffDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matBuffDesc, NULL, &m_matBuffer);
	if (FAILED(result))
		return false;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_SS);
	if (FAILED(result))
		return false;
	return true;
}

void TextureShader::ShutdownShader()
{
	if (m_SS)
	{
		m_SS->Release();
		m_SS = 0;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	if (m_matBuffer)
	{
		m_matBuffer->Release();
		m_matBuffer = 0;
	}
}

void TextureShader::OutputShaderErrorMsg(ID3D10Blob* msg, HWND hwnd, WCHAR* file)
{
	char* compileErrors;
	unsigned long bufferSize;
	ofstream fout;

	compileErrors = (char*)(msg->GetBufferPointer());

	bufferSize = msg->GetBufferSize();

	fout.open("texture-shader-error.txt");
	for (int i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	msg->Release();
	msg = 0;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", file, MB_OK);
	return;
}

bool TextureShader::SetShaderParams(ID3D11DeviceContext* ctxt, XMMATRIX world, XMMATRIX view, XMMATRIX proj, XMMATRIX object, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	MatrixBuffer* dataPtr;
	unsigned int bufferNum;

	object = XMMatrixTranspose(object);
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	result = ctxt->Map(m_matBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (FAILED(result))
		return false;
	dataPtr = (MatrixBuffer*)mappedSubresource.pData;
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;
	dataPtr->object = object;
	ctxt->Unmap(m_matBuffer, 0);
	bufferNum = 0;
	ctxt->VSSetConstantBuffers(bufferNum, 1, &m_matBuffer);
	ctxt->PSSetShaderResources(0, 1, &texture);
	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* ctxt, int indexCount)
{
	ctxt->IASetInputLayout(m_layout);

	ctxt->VSSetShader(m_vertexShader, NULL, 0);
	ctxt->PSSetShader(m_pixelShader, NULL, 0);
	ctxt->PSSetSamplers(0, 1, &m_SS);
	ctxt->DrawIndexed(indexCount, 0, 0);

	return;
}