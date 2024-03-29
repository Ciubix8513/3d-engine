#include "MaterialComponent.h"



std::vector<const type_info*> Engine::MaterialComponent::GetRequieredComponents()
{
	return {&typeid(Engine::MeshComponent)};
}

//Initialisation function
void Engine::MaterialComponent::Initialise(std::vector<Component*> Comps, D3d** d3d, ULONG entityUUID)
{
	m_D3dPtr = d3d;
	m_mesh = (MeshComponent*)Comps[0];
	m_layout = 0;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_VSBnum = 0;
	m_PSBnum = 0;
	EntityUUID = entityUUID;
	return;
}

std::string Engine::MaterialComponent::GetName()
{
	return "Material";
}

//Cleanup function
void Engine::MaterialComponent::Shutdown()
{
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
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
	for (size_t i = 0; i < m_buffers.size(); i++)
		if (m_buffers[i].buffer)
		{
			m_buffers[i].buffer->Release();
			m_buffers[i].buffer = 0;
		}
	for (size_t i = 0; i < m_samplerBuffer.size(); i++)
		if (m_samplerBuffer[i].sampler)
		{
			m_samplerBuffer[i].sampler->Release();
			m_samplerBuffer[i].sampler = 0;
		}
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
std::vector<std::string> Engine::MaterialComponent::GetWordsFromFile(std::string  fileName)
{
	std::ifstream f(fileName);
	if (!f.is_open())
		throw std::exception("Shader file does not exist");
	std::string tmp1;
	std::vector<std::string> Words;
	while (f.good())
	{	
		//f >> tmp1;
		std::getline(f, tmp1);
		tmp1 += '\n';
		Words.push_back( tmp1);		
	}
	f.close();
	return Words;
}

//Getting the byte width
//size_t ByteWidth = 0;
//if ((*(iterators[i] + 1)).substr(0, 6) == "float ")
//ByteWidth = 4;
//else if ((*(iterators[i] + 1)).substr(0, 6) == "float2")
//ByteWidth = 8;
//else if ((*(iterators[i] + 1)).substr(0, 6) == "float3")
//ByteWidth = 12;
//else if ((*(iterators[i] + 1)).substr(0, 6) == "float4")
//ByteWidth = 16;
//else if ((*(iterators[i] + 1)).substr(0, 6) == "matrix")
//ByteWidth = 64;

bool Engine::MaterialComponent::PreProcessShader(std::string fileName)
{
	std::vector <std::string> Words;
	try
	{
		Words = GetWordsFromFile(fileName);

	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " + (std::string)(e.what()) + "\n"; //TODO: implement internal error handling
		return false;
	}

	std::vector<std::vector<std::string>::iterator> iterators;
	//Processing single variables
	//Getting list of iterators
	for (auto i = Words.begin(); i != Words.end(); i++)
		if ((*i)[0] == '[') //Check if the word starts with a [
			if ((*i).substr(1, 9) == "WriteVar(")
				iterators.push_back(i);

	//List of all variable names
	std::vector<std::string> VarNames;

	for (size_t i = 0; i < iterators.size(); i++)
	{
		std::string tmp = (*iterators[i]).substr(11, (*iterators[i]).find_last_of('\"') - 11); //Geting the name of a variable
		//Probably very stupid but
		for (auto j = VarNames.begin(); j != VarNames.end(); j++)
			if ((*j) == tmp)
			{
				throw std::exception("Variable names must be unique\n");
				return false;
			}
		
		VarNames.push_back(tmp);
		(*iterators[i]) = "cbuffer " + tmp + "\n{\n    ";
		(*(iterators[i] + 1)) = (*(iterators[i] + 1)) + "};\n";

	}

	//A very stupid fix but should work
	std::vector <std::string> NewWords;
	for (std::string s : Words)
	{
		std::string tmp;
		bool Space = false;
		for (size_t i = 0; i < s.size(); i++)		
			if (s[i] == '\t')
			{				
				tmp += "    ";
				
				Space = true;
			}
			else if (s[i] == ' ')
			{
				tmp += ' ';
				Space = true;
			}
			else
			{
				Space = false;
				tmp += s[i];
			}		
		NewWords.push_back( tmp);
	}
	//Creating processed file
	std::ofstream f(fileName + ".processed");
	for (size_t i = 0; i < NewWords.size(); i++)
		f << NewWords[i];
	f.close();

	//this might be dumb but
	Words = GetWordsFromFile(fileName + ".processed");

	iterators.clear();
	//finding all cbuffers
	for (auto i = Words.begin(); i != Words.end(); i++)
		if ((*i).substr(0,7) == "cbuffer")			
				iterators.push_back(i);

	size_t ByteWidth = 0;
	std::string tmp;
	int NumVars = 0;

	for (auto i = iterators.begin(); i != iterators.end(); i++)
	{
		while ((**(i) != "};\n"))
		{
			tmp = (**(i)).substr(0, 10);
			if (tmp == "    float ")
				ByteWidth += 4;
			else if (tmp == "    float2")
				ByteWidth += 8;
			else if (tmp == "    float3")
				ByteWidth += 12;
			else if (tmp == "    float4")
				ByteWidth += 16;	
			else if (tmp == "    matrix")
				ByteWidth += 64;

			(*i) ++;
			NumVars++;
			if (NumVars == 1000000000000000)			
				std::cout << "I fucked up\n";
			
		}
		if(ByteWidth == 0)
		{
			throw std::exception("Wrong shader syntax");
		}

		(*(i)) -= NumVars;
		(**(i)) = (std::string)(**(i)+"//" + std::to_string(ByteWidth) + "\n");
		NumVars = 0;
		ByteWidth = 0;
	}

	std::ofstream f1(fileName + ".processed");
	for (size_t i = 0; i < Words.size(); i++)
		f1 << Words[i];
	f1.close();

	return true;
}

void Engine::MaterialComponent::Render()
{
	//Setting index and vertex buffers
	m_mesh->Render();

	//Setting buffers
	if (m_VSBnum > 0)
	{
		ID3D11Buffer** VSBs = new ID3D11Buffer * [m_VSBnum];
		int VSBnum = 0;
		for (Engine::MaterialComponent::Buffer b : m_buffers)
			if (b.type == VertexShader)
			{
				VSBs[VSBnum] = b.buffer;
				VSBnum++;
			}
		(*m_D3dPtr)->getDeviceContext()->VSSetConstantBuffers(0, m_VSBnum, VSBs);

	}
	if (m_PSBnum > 0)
	{
		ID3D11Buffer** PSBs = new ID3D11Buffer * [m_PSBnum];
		int PSBnum = 0;
		for (Engine::MaterialComponent::Buffer b : m_buffers)
			if (b.type == PixelShader)
			{
				PSBs[PSBnum] = b.buffer;
				PSBnum++;
			}

		(*m_D3dPtr)->getDeviceContext()->PSSetConstantBuffers(0, m_PSBnum, PSBs);
	}

	//Setting input layout
	(*m_D3dPtr)->getDeviceContext()->IASetInputLayout(m_layout);
	
	//Setting shaders
	(*m_D3dPtr)->getDeviceContext()->VSSetShader(m_vertexShader, 0, 0);
	(*m_D3dPtr)->getDeviceContext()->PSSetShader(m_pixelShader, 0, 0);

	//Setting samplers
	for (size_t i = 0; i < m_samplerBuffer.size(); i++)
	{
		if (m_samplerBuffer[i].type == VertexShader)
			(*m_D3dPtr)->getDeviceContext()->VSSetSamplers(m_samplerBuffer[i].samplerNum, 1, &m_samplerBuffer[i].sampler);
		else
			(*m_D3dPtr)->getDeviceContext()->PSSetSamplers(m_samplerBuffer[i].samplerNum, 1, &m_samplerBuffer[i].sampler);
	}
	//Setting textures
	for (size_t i = 0; i < m_textureBuffer.size(); i++)
	{
		if (m_textureBuffer[i].type == VertexShader)
			if (m_textureBuffer[i].texture != nullptr)
				(*m_D3dPtr)->getDeviceContext()->VSSetShaderResources(m_textureBuffer[i].TextureNum, 1, m_textureBuffer[i].texture->GetTexture());
		if (m_textureBuffer[i].type == PixelShader)
			if(m_textureBuffer[i].texture != nullptr)
				(*m_D3dPtr)->getDeviceContext()->PSSetShaderResources(m_textureBuffer[i].TextureNum, 1, m_textureBuffer[i].texture->GetTexture());			
	}
	
	//Drawing the mesh!
	(*m_D3dPtr)->getDeviceContext()->DrawIndexed((m_mesh)->m_Model.indexCount, 0, 0);


	return;
}

bool Engine::MaterialComponent::InitShader(std::string vsFilename, std::string  psFilename, std::string ShaderName, UINT FLAGS1, UINT FLAGS2)
{
	HRESULT result;
	ID3D10Blob* errorMsg;
	ID3D10Blob* vertexshaderBuff;
	ID3D10Blob* pixelShaderBuff;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matBuffDesc, lighBuffDesc, cameraBuffDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMsg = 0;
	vertexshaderBuff = 0;
	pixelShaderBuff = 0;
	//Preprocessing shaders
	PreProcessShader(vsFilename);
	PreProcessShader(psFilename);
	CA2W tmp1((vsFilename + ".processed").c_str());
	auto tmp = tmp1.m_psz;
	
	//Compiling vertex shawder
	result = D3DCompileFromFile(tmp, NULL, NULL,(( ShaderName + "VertexShader").c_str()), "vs_5_0", FLAGS1, FLAGS2, &vertexshaderBuff, &errorMsg);
	if(FAILED(result))
	{
		if (errorMsg)
			throw std::exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw std::exception((vsFilename + (std::string)" does not exist").c_str());
		return false;
	}
	//Compiling pixel shader
	result = D3DCompileFromFile(CA2W((psFilename + ".processed").c_str()).m_psz, NULL, NULL, ((ShaderName + "PixelShader").c_str()), "ps_5_0", FLAGS1, FLAGS2, &pixelShaderBuff, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			throw std::exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw std::exception((psFilename + (std::string)" does not exist").c_str());
		return false;
	}

	//Creating vertex shader
	result = (*m_D3dPtr)->getDevice()->CreateVertexShader(vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		throw std::exception("Could not create vertex shader");
		return false;
	}

	//Creating pixel shader
	result = (*m_D3dPtr)->getDevice()->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		throw std::exception("Could not create pixel shader");
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
	result = (*m_D3dPtr)->getDevice()->CreateInputLayout(polygonLayout, numElements, vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		throw std::exception("Failed to create input layout");
		return false;
	}

	//Releasing buffers
	vertexshaderBuff->Release();
	pixelShaderBuff->Release();
	vertexshaderBuff = 0;
	pixelShaderBuff = 0;


	//Getting buffer data 
	std::vector<std::string> Words = GetWordsFromFile(vsFilename + ".processed");

	std::string name;	
	size_t count = 0;
	size_t SamplerCount = 0;
	size_t TextureCount = 0;

	//Vertex shader buffer creation
	for (size_t i = 0; i < Words.size(); i++) 
	{
		std::string tmp = Words[i].substr(0, 8);
		if (tmp == "cbuffer ")// Finding all buffers
		{
			//Creating buffer
			name = Words[i].substr(8);
			name = name.substr(0, name.size() - 1);

			Engine::MaterialComponent::Buffer b = Buffer(name, count, VertexShader);

			if (!b.CreateBuffer((*m_D3dPtr)->getDevice(), atoi(Words[i + 1].substr(2).c_str())))
			{
				throw std::exception(("Failed to create " + name + " buffer").c_str());
				return false;
			}

			
			m_buffers.push_back(b);
			m_VSBnum++;
			count++;
		}
		else if (tmp == "SamplerS") // Check if it's a sampler
		{
			name = Words[i].substr(13);
			name = name.substr(0, name.size() - 2);
			Engine::MaterialComponent::Sampler s;
			s.name = name;
			s.type = VertexShader;
			s.samplerNum = SamplerCount;
			s.CreateSampler((*m_D3dPtr)->getDevice());
			m_samplerBuffer.push_back(s);
			SamplerCount++;
		}
		else if (tmp == "Texture2")
		{
			name = Words[i].substr(10);
			name = name.substr(0, name.size() - 2);
			Engine::MaterialComponent::Tex t;
			t.name = name;
			t.type = VertexShader;
			t.TextureNum = TextureCount;
			m_textureBuffer.push_back(t);
			TextureCount++;
		}
	}

	count = 0;
	SamplerCount = 0;
	TextureCount = 0;
	Words = GetWordsFromFile(psFilename + ".processed");

	//Pixel shader buffer creation
	for (size_t i = 0; i < Words.size(); i++) 
	{

		std::string tmp = Words[i].substr(0,8);

		if (tmp == "cbuffer ")// Finding all buffers
		{
			//Creating buffer
			name = Words[i].substr(8);
			name = name.substr(0, name.size() - 1);

			Engine::MaterialComponent::Buffer b = Buffer(name,count,PixelShader);		

			if (!b.CreateBuffer((*m_D3dPtr)->getDevice(), atoi(Words[i + 1].substr(2).c_str())))
			{
				throw std::exception(("Failed to create " + name + " buffer").c_str());
				return false;
			}	
			m_buffers.push_back(b);
			m_PSBnum++;
			count++;
		}
		else if (tmp == "SamplerS") // Check if it's a sampler
		{
			name = Words[i].substr(13);
			name = name.substr(0, name.size() - 2);
			Engine::MaterialComponent::Sampler s;
			s.name = name;
			s.type = PixelShader;
			s.samplerNum = SamplerCount;
			s.CreateSampler((*m_D3dPtr)->getDevice());
			m_samplerBuffer.push_back(s);
			SamplerCount++;
		}
		else if(tmp == "Texture2")
		{
			name = Words[i].substr(10);
			name = name.substr(0, name.size() - 2);
			Engine::MaterialComponent::Tex t;
			t.name = name;
			t.type = PixelShader;
			t.TextureNum = TextureCount;
			t.texture = nullptr;
			m_textureBuffer.push_back(t);
			TextureCount++;
		}
	}

	return true;
}

std::string Engine::MaterialComponent::GetShaderErrorMsg(ID3D10Blob* msg)
{
	std::string compileErrors;
	compileErrors = (char*)(msg->GetBufferPointer());
	msg->Release();
	msg = 0;
	return compileErrors;
}

Engine::MaterialComponent::Buffer::Buffer(std::string Name, size_t BufferNum, ShaderType Type)
{
	name = Name;
	bufferNum = BufferNum;
	type = Type;
	buffer = 0;
}

bool Engine::MaterialComponent::Buffer::CreateBuffer(ID3D11Device* device,size_t ByteWidth)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = ByteWidth;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	auto result = device->CreateBuffer(&Desc, NULL, &this->buffer);
	if (FAILED(result))
		return false;
	
	return true;
}
bool Engine::MaterialComponent::Buffer::CreateBuffer(ID3D11Device* device, size_t ByteWidth, D3D11_SUBRESOURCE_DATA* InitialData)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = ByteWidth;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	auto result = device->CreateBuffer(&Desc, InitialData, &this->buffer);
	if (FAILED(result))
		return false;

	return true;
}

#pragma region Set functions
bool Engine::MaterialComponent::SetFloat(std::string Name, float Data)
{
	for(Buffer B: m_buffers)	
		if(B.name == Name)
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			*(float*)data.pData = Data;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}

	return false;
}
bool Engine::MaterialComponent::SetVector2(std::string Name, Vector2 Data)
{
	for (Buffer B : m_buffers)
		if (B.name == Name)
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			*(Vector2*)data.pData = Data;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}

	return false;
}
bool Engine::MaterialComponent::SerVector3(std::string Name, Vector3 Data)
{
	for (Buffer B : m_buffers)
		if (B.name == Name)
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			*(Vector3*)data.pData = Data;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}

	return false;
}
bool Engine::MaterialComponent::SetVector4(std::string Name, Vector4 Data)
{
	for (Buffer B : m_buffers)
		if (B.name == Name)
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			*(Vector4*)data.pData = Data;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}

	return false;
}
bool Engine::MaterialComponent::SetMatrix(std::string Name, Matrix4x4 Data)
{
	for (Buffer B : m_buffers)
		if (B.name == Name)
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			*(Matrix4x4*)data.pData = Data;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}

	return false;
}

bool Engine::MaterialComponent::SetMatrixBuffer(MatrixBuffer Data)
{
	for (Buffer B : m_buffers)
		if (B.name == "MatrixBuffer")
		{
			HRESULT res;
			D3D11_MAPPED_SUBRESOURCE data;
			res = (*m_D3dPtr)->getDeviceContext()->Map(B.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
			if (FAILED(res))
				return false;
			MatrixBuffer* BufferPtr = (MatrixBuffer *) data.pData;
			BufferPtr->projectionMatrix = Data.projectionMatrix.Transposed();
			BufferPtr->viewMatrix = Data.viewMatrix.Transposed();
			BufferPtr->worldMatrix = Data.worldMatrix;
			(*m_D3dPtr)->getDeviceContext()->Unmap(B.buffer, 0);
			return true;
		}


	return false;
}


bool Engine::MaterialComponent::SetSampler(std::string Name, ID3D11SamplerState* data)
{
	for (size_t i = 0; i < m_samplerBuffer.size(); i++)
		if (m_samplerBuffer[i].name == Name)
		{
			m_samplerBuffer[i].sampler->Release();
			m_samplerBuffer[i].sampler = 0;
			m_samplerBuffer[i].sampler = data;			
			return true;
		}
	return false;
}
bool Engine::MaterialComponent::SetTexture(std::string Name, Texture* data, bool deleteData)
{
	for (size_t i = 0; i < m_textureBuffer.size(); i++)
		if (m_textureBuffer[i].name == Name)
		{			
			if (deleteData)
				m_textureBuffer[i].texture->Shutdown();			
			m_textureBuffer[i].texture = 0;
			m_textureBuffer[i].texture = data;
			return true;
		}


	return false;
}
#pragma endregion

bool Engine::MaterialComponent::Sampler::CreateSampler(ID3D11Device* device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&desc, &this->sampler);
	if (FAILED(result)) 
	{
		throw std::exception("Failed to create sampler state");
		return false;
	}
	return true;
}
