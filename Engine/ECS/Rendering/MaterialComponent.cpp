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
std::vector<std::string> Engine::MaterialComponent::GetWordsFromFile(WCHAR* fileName)
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
bool Engine::MaterialComponent::PreProcessShader(WCHAR* fileName)
{
	std::vector <std::string> Words;
	try
	{
		Words = GetWordsFromFile(fileName);

	}
	catch (const std::exception& e)
	{
		std::cout << "Caught exception: " + (std::string)(e.what()) + "\n"; //TODO: implement internal error handling
	}

	std::vector<std::vector<std::string>::iterator> iterators;
	//Getting list of iterators
	for (auto i = Words.begin(); i != Words.end(); i++)
		if ((*i)[0] == '[') //Check if the word starts with a [
			if ((*i).substr(1, 9) == "WriteVar(")
				iterators.push_back(i);

	for (size_t i = 0; i < iterators.size(); i++)
	{
		std::string tmp = (*iterators[i]).substr(11, (*iterators[i]).find_last_of('\"') - 11); //Geting the name of a variable
		(*iterators[i]) = "cbuffer " + tmp + "\n{\n\t";
		(*(iterators[i] + 1)) = (*(iterators[i] + 1)) + "};\n";

	}
	std::ofstream f(((std::wstring)fileName + (std::wstring)L".processed"));
	for (size_t i = 0; i < Words.size(); i++)
		f << Words[i];
	f.close();


	return true;
}
// = D3D10_SHADER_ENABLE_STRICTNESS
bool Engine::MaterialComponent::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, std::string ShaderName, UINT FLAGS1, UINT FLAGS2)
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
	//Preprocessing shaders
	PreProcessShader(vsFilename);
	PreProcessShader(psFilename);


	//Compiling vertex shader
	result = D3DCompileFromFile((((std::wstring)vsFilename + (std::wstring)L".proceced")).c_str(), NULL, NULL,(( ShaderName + "VertexShader").c_str()), "vs_5_0", FLAGS1, FLAGS2, &vertexshaderBuff, &errorMsg);
	if(FAILED(result))
	{
		if (errorMsg)
			throw std::exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw std::exception(((char*)vsFilename + (std::string)" does not exist").c_str());
		return false;
	}
	//Compiling pixel shader
	result = D3DCompileFromFile((((std::wstring)psFilename + (std::wstring)L".proceced")).c_str(), NULL, NULL, ((ShaderName + "PixelShader").c_str()), "ps_5_0", FLAGS1, FLAGS2, &pixelShaderBuff, &errorMsg);
	if (FAILED(result))
	{
		if (errorMsg)
			throw std::exception(GetShaderErrorMsg(errorMsg).c_str());
		else
			throw std::exception(((char*)psFilename + (std::string)" does not exist").c_str());
		return false;
	}

	//Creating vertex shader
	result = device->CreateVertexShader(vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		throw std::exception("Could not create vertex shader");
		return false;
	}

	//Creating pixel shader
	result = device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), NULL, &m_pixelShader);
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
	result = device->CreateInputLayout(polygonLayout, numElements, vertexshaderBuff->GetBufferPointer(), vertexshaderBuff->GetBufferSize(), &m_layout);
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
	std::vector<std::string> Words = GetWordsFromFile(vsFilename);

	for (size_t i = 0; i < Words.size(); i++)
	{
		if(Words[i] == "cbuffer")// Finding all buffers
		{
			if (Words[i + 1] == "MatrixBuffer") {}
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
