#include "Bitmap.h"

Bitmap::Bitmap()
{
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
    m_Texture = 0;
}

Bitmap::Bitmap(const Bitmap&)
{
}

Bitmap::~Bitmap()
{
}

bool Bitmap::Init(ID3D11Device* device, int screenWidth, int screenHeight, char* texFile, int bitmapWidth, int bitmapHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_bimapWidth = bitmapWidth;
    m_bitmapHeight = bitmapHeight;

    m_PreviosPosX = -1;
    m_PreviosPosY = -1;

    bool result = Initbuffers(device);
    if (!result) 
    {
        std::cout << "could not  initialize buffers" << std::endl;
        return false;        
    }
    result = LoadTexture(device, texFile);
    if (!result) 
    {
        std::cout << "could not  Load texture" << std::endl;

        return false;
    
    }
    return true;
}

void Bitmap::ShutDown()
{
    Releasetexture();
    ShutDownBuffers();
    return;   
}

bool Bitmap::Render(ID3D11DeviceContext* ctxt, int posX, int posY)
{
    bool result;
    result = UpdateBuffers(ctxt,posX,posY);
    if (!result)
        return false;
    RenderBuffers(ctxt);
    return true;
}

int Bitmap::GetIndexCount()
{
    return m_indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
    return m_Texture->GetTexture();
}

bool Bitmap::Initbuffers(ID3D11Device* device)
{
    BitmapVertex* vertices;
    unsigned long* indecies;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount = 6;
    m_indexCount = m_vertexCount;

    vertices = new BitmapVertex[m_vertexCount];
    if (!vertices)
        return false;
    indecies = new unsigned long[m_indexCount];
    if (!indecies)
        return indecies;

    memset(vertices, 0, (sizeof(BitmapVertex) * m_vertexCount));

    for (int i = 0; i < m_indexCount; i++)
    {
        indecies[i] = i;
    }

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
    indexBufferDesc.BindFlags = 0x2L;//0x2L = bind index buffer
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;    

    indexData.pSysMem = indecies;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

   result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
        return false;


    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(BitmapVertex) * m_vertexCount;
    vertexBufferDesc.BindFlags = 0x1L; // 0x1L = bind vertex buffer
    vertexBufferDesc.CPUAccessFlags = 0x10000L;//0x10000L = cpu acces write
    vertexBufferDesc.MiscFlags  = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
        return false;
    
    delete[] vertices;
    vertices = 0;
    delete[] indecies;
    indecies = 0;
    return true;
}

void Bitmap::ShutDownBuffers()
{
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }
    return;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* ctxt, int posX, int posY)
{
    float left, right, top, bottom;
    BitmapVertex* vertices;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    BitmapVertex* verticesPtr;
    HRESULT result;

    if (posX == m_PreviosPosX && posY == m_PreviosPosY)
    { 



        return true;
    }
    m_PreviosPosX = posX;
    m_PreviosPosY = posY;
    left = (float)((m_screenWidth / 2) * -1) + (float)posX;
    right = left + (float)m_bimapWidth;
    top = (float)((m_screenHeight / 2)) + (float)posY;
    bottom = top - (float)m_bitmapHeight;
    
    vertices = new BitmapVertex[m_vertexCount];
    if (!vertices)
        return false;

    vertices[0].position = XMFLOAT3(left, top, 0.0f);
    vertices[0].Texture = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
    vertices[1].Texture = XMFLOAT2(1, 1);// XMFLOAT2(1.0f, 1.0f);
    
    vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
    vertices[2].Texture = XMFLOAT2(0.0f, 1.0f);

    vertices[3].position = XMFLOAT3(left, top, 0.0f);
    vertices[3].Texture = XMFLOAT2(0.0f, 0.0f);

    vertices[4].position = XMFLOAT3(right, top, 0.0f);
    vertices[4].Texture = XMFLOAT2(1.0f, 0.0f);

    vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
    vertices[5].Texture = XMFLOAT2(1.0f, 1.0f);
    
    result = ctxt->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
    if (FAILED(result))
        return false;
    verticesPtr = (BitmapVertex*)mappedResource.pData;

   memcpy(verticesPtr, (void*)vertices, (sizeof(BitmapVertex) * m_vertexCount));
   

    ctxt->Unmap(m_vertexBuffer, 0);



    delete[] vertices;

    vertices = 0;

    return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* ctxt)
{
    unsigned int stride, offset;
    stride = sizeof(BitmapVertex);
    offset = 0;

    ctxt->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    ctxt->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    ctxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    return;
}

bool Bitmap::LoadTexture(ID3D11Device* device, char* file)
{
    bool result;

    m_Texture = new Texture;
    if (!m_Texture)
    {
        std::cout << "could not  create texture object" << std::endl;
        return false;
    }
    ID3D11DeviceContext* ctxt;
    
   device->GetImmediateContext(&ctxt);

   result = m_Texture->Init(device, ctxt, (char*)file);
   if (!result) 
   {
       std::cout << "could not  initialize texture object" << std::endl;
       return false;
   }
    return true;
}

void Bitmap::Releasetexture()
{
    if (m_Texture)
    {
        m_Texture->Shutdown();
        m_Texture = 0;
    }
    return;
}
