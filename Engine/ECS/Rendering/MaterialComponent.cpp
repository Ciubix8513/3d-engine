#include "MaterialComponent.h"

std::vector<const type_info*> Engine::MaterialComponent::GetRequieredComponents()
{
	return {&typeid(Engine::MeshComponent)};
}

//Initialisation function
void Engine::MaterialComponent::Initialise(std::vector<Component*> Comps, D3d** d3d)
{
	m_D3dPtr = d3d;
	m_mesh = (MeshComponent*)Comps[0];
	m_layout = 0;
	m_vertexShader = 0;
	m_pixelShader = 0;

	return;
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
		if (m_buffers[i].Buffer)
		{
			m_buffers[i].Buffer->Release();
			m_buffers[i].Buffer = 0;
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

	//Creating processed file
	std::ofstream f(fileName + ".processed");
	for (size_t i = 0; i < Words.size(); i++)
		f << Words[i];
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
			if (tmp == "    float")
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
	//SetStruct("MatrixBuffer",)
	//Seting shader parameters	
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	for (size_t i = 0; i < m_buffersBuffer.size(); i++)	
		if (m_buffersBuffer[i].Changed) 
		{
			m_buffersBuffer[i].Changed = false;
			result = (*m_D3dPtr)->getDeviceContext()->Map(m_buffers[i].Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
			if (FAILED(result))
				throw std::exception(("Failed to map " + m_buffersBuffer[i].name).c_str());
			switch (m_buffersBuffer[i].type)
			{

			case Float:
				*((float*)mappedSubresource.pData) = m_buffersBuffer[i].data.Float;
				break;
			case vector2:
				*((EngineMath::Vector2*)mappedSubresource.pData) = m_buffersBuffer[i].data.Vector2;

				break;
			case vector3:
				*((EngineMath::Vector3*)mappedSubresource.pData) = m_buffersBuffer[i].data.Vector3;

				break;
			case vector4:
				*((EngineMath::Vector4*)mappedSubresource.pData) = m_buffersBuffer[i].data.Vector4;

				break;
			case Matrix:
				*((Matrix4x4*)mappedSubresource.pData) = m_buffersBuffer[i].data.Matrix;
				break;
			case Struct:
				*((BufferClass*)mappedSubresource.pData) = *m_buffersBuffer[i].data.Buffer.Buffer; //Most likely won't work
				break;

			}
			(*m_D3dPtr)->getDeviceContext()->Unmap(m_buffers[i].Buffer, 0);
			if (m_buffers[i].type == VertexShader)
				(*m_D3dPtr)->getDeviceContext()->VSSetConstantBuffers(m_buffers[i].bufferNum, 1, &m_buffers[i].Buffer);
			else
				(*m_D3dPtr)->getDeviceContext()->PSSetConstantBuffers(m_buffers[i].bufferNum, 1, &m_buffers[i].Buffer);
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
	
	//Vertex shader buffer creation
	for (size_t i = 0; i < Words.size(); i++) 
	{
		std::string tmp = Words[i].substr(0, 8);
		if (tmp == "cbuffer ")// Finding all buffers
		{
			//Creating buffer
			name = Words[i].substr(8);
			Buffer b;
			BufferBuffer b1(name);

			b.Buffer = 0;
			b.type = VertexShader;
			b.bufferNum = count;
			if (!b.CreateBuffer((*m_D3dPtr)->getDevice(), atoi(Words[i + 1].substr(2).c_str())))
			{
				throw std::exception(("Failed to create " + name + " buffer").c_str());
				return false;
			}
			m_buffersBuffer.push_back(b1);
			m_buffers.push_back(b);
			count++;
		}
		else if(tmp == "sampler") // Check if it's a sampler
		{
			name = Words[i].substr(9);
			Sampler s;
			s.name = name;
			s.type = VertexShader;
			s.samplerNum = SamplerCount;
			s.CreateSampler((*m_D3dPtr)->getDevice());
			m_samplerBuffer.push_back(s);
			SamplerCount++;
		}
	}

	count = 0;
	SamplerCount = 0;
	Words = GetWordsFromFile(psFilename + ".processed");

	//Pixel shader buffer creation
	for (size_t i = 0; i < Words.size(); i++) 
	{

		std::string tmp = Words[i].substr(0, 8);
		if (tmp == "cbuffer ")// Finding all buffers
		{
			//Creating buffer
			name = Words[i].substr(8);
			Buffer b;
			BufferBuffer b1(name);


			b.Buffer = 0;
			b.type = PixelShader;
			b.bufferNum = count;

			if (!b.CreateBuffer((*m_D3dPtr)->getDevice(), atoi(Words[i + 1].substr(2).c_str())))
			{
				throw std::exception(("Failed to create " + name + " buffer").c_str());
				return false;
			}
			m_buffersBuffer.push_back(b1);
			m_buffers.push_back(b);
			count++;
		}
		else if (tmp == "sampler") // Check if it's a sampler
		{
			name = Words[i].substr(9);
			Sampler s;
			s.name = name;
			s.type = PixelShader;
			s.samplerNum = SamplerCount;
			m_samplerBuffer.push_back(s);
			SamplerCount++;
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

bool Engine::MaterialComponent::Buffer::CreateBuffer(ID3D11Device* device,size_t ByteWidth)
{
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth = ByteWidth;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.StructureByteStride = 0;

	auto result = device->CreateBuffer(&Desc, NULL, &this->Buffer);
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

	auto result = device->CreateBuffer(&Desc, InitialData, &this->Buffer);
	if (FAILED(result))
		return false;
	return true;
}

#pragma region Set functions
void Engine::MaterialComponent::SetFloat(std::string name, float data)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)	
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Float = data;
			m_buffersBuffer[i].type = Float;
		}
	
}
void Engine::MaterialComponent::SetVector2(std::string name, Vector2 data)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Vector2 = data;
			m_buffersBuffer[i].type = vector2;
		}
}
void Engine::MaterialComponent::SerVector3(std::string name, Vector3 data)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Vector3 = data;
			m_buffersBuffer[i].type = vector3;
		}
}
void Engine::MaterialComponent::SetVector4(std::string name, Vector4 data)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Vector4 = data;
			m_buffersBuffer[i].type = vector4;
		}
}
void Engine::MaterialComponent::SetMatrix(std::string name, Matrix4x4 data)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Matrix = data;
			m_buffersBuffer[i].type = Matrix;
		}
}
void Engine::MaterialComponent::SetSampler(std::string name, ID3D11SamplerState* data)
{
	for (size_t i = 0; i < m_samplerBuffer.size(); i++)
		if (m_samplerBuffer[i].name == name)
		{
			m_samplerBuffer[i].sampler->Release();
			m_samplerBuffer[i].sampler = 0;
			m_samplerBuffer[i].sampler = data;			
		}
}
void Engine::MaterialComponent::SetStruct(std::string name, BufferClass* data, const type_info* bufferType)
{
	for (size_t i = 0; i < m_buffersBuffer.size(); i++)
		if (m_buffersBuffer[i].name == name)
		{
			m_buffersBuffer[i].data.Buffer.Buffer = data;
			m_buffersBuffer[i].data.Buffer.type = bufferType;
			m_buffersBuffer[i].type = Struct;
		}
	
}
#pragma endregion

/*// I am very unsure about this function
bool Engine::MaterialComponent::SetFloat(ID3D11DeviceContext* ctxt, std::string name, float data)
{
	//Finding the buffer
	size_t BufferID = -1;
	for (size_t i = 0; i < m_buffers.size(); i++)
		if (m_buffers[i].BufferName == name)
		{
			BufferID = i;
			break;
		}
	if(BufferID == -1)
	{
		if (m_buffers.size() == 0)
			throw std::exception("No buffers exist in this shader");
		else
			throw std::exception(((std::string)("No buffers with name = " + name + " exis in this shader")).c_str());
		return false;
	}

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	result = ctxt->Map(m_buffers[BufferID].Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (FAILED(result))
		return false;

	*(float*)mappedSubresource.pData = data;
	ctxt->Unmap(m_buffers[BufferID].Buffer, 0);


	if (m_buffers[BufferID].type == VertexShader)
		ctxt->VSSetConstantBuffers(m_buffers[BufferID].bufferNum, 0, &m_buffers[BufferID].Buffer);
	else
		ctxt->PSSetConstantBuffers(m_buffers[BufferID].bufferNum, 0, &m_buffers[BufferID].Buffer);
	return true;
}*/

Engine::MaterialComponent::BufferBuffer::BufferBuffer(std::string name)
{
	name = name;
}
Engine::MaterialComponent::ShaderBufferType::ShaderBufferType()
{
	Float = 0;
	Vector2 = EngineMath::Vector2(0, 0);
	Vector3 = EngineMath::Vector3(0, 0,0);
	Vector4 = EngineMath::Vector4(0, 0,0,0);
	Matrix = EngineMath::Matrix4x4();
}
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
