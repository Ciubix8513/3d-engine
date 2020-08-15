#include "Text.h"

Text::Text()
{
    m_Font = 0;
    m_FontShader = 0;
    sentence0 = 0;
    sentence1 = 0;
    m_screenH = 0;
    m_screenW = 0;
    m_baseViewMat = XMMatrixIdentity();

}

Text::Text(const Text&)
{
}

Text::~Text()
{
}

bool Text::Init(ID3D11Device* device, ID3D11DeviceContext* ctxt, HWND hwnd, int scrW, int scrH, XMMATRIX viewMat, int numChars)
{
    bool res;


    m_screenH = 0;
    m_screenW = 0;
    m_baseViewMat = XMMatrixIdentity();
    m_baseViewMat = viewMat;

    m_screenW = scrW;
    m_screenH = scrH;


    m_Font = new Font;
    if (!m_Font)
        return false;

    res = m_Font->Init(device, ctxt, (char*)"../Engine/data/FontData.FNT", (char*)"../Engine/data/FontImg.TGA", numChars);
    if(!res)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(hwnd, L"Could not Initialize Font object", L"Application Initialization error", MB_OK);

        return false;
    }


    m_FontShader = new FontShader;
    if (!m_FontShader)
        return false;
    res =m_FontShader->Init(device, hwnd);
    if (!res)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(hwnd, L"Could not Initialize Font shader object", L"Application Initialization error", MB_OK);
        return false;
    }

    res = InitSentence(&sentence0, 16, device);
    if (!res)
        return false;
    res = InitSentence(&sentence1, 16, device);
    if (!res)
        return false;

    res = UpdateSentence(sentence0, (char*)"#", 100, -100, 1.0f, 1.0f, 1.0f, ctxt);
    if (!res)
        return false;

    res = UpdateSentence(sentence1, (char*)"bye", 100,-80, 1.0f, 1.0f, 0.0f, ctxt);
    if (!res)
        return false;

    return true;
}

void Text::ShutDown()
{
    Releasesentence(&sentence0);
    Releasesentence(&sentence1);

    if (m_FontShader) 
    {
        m_FontShader->ShutDown();
        delete m_FontShader;
        m_FontShader = 0;
    }

    if (m_Font)
    {
        m_Font->ShutDown();
        delete m_Font;
        m_Font = 0;
    }
    return;
}

bool Text::Render(ID3D11DeviceContext* ctxt, XMMATRIX world, XMMATRIX ortho)
{
    bool res;

    res = RenderSentence(ctxt, sentence0, world, ortho);
    if (!res)
        return false;

    res = RenderSentence(ctxt, sentence1, world, ortho);
    if (!res)
        return false;
    return true;
}

bool Text::SetMousePosition(int mouseX,int mouseY, ID3D11DeviceContext* ctxt)
{
    char tempString[16];
    char mouseString[16];
    bool result;
    _itoa_s(mouseX, tempString, 10);//convert int to string
    strcpy_s(mouseString, "MouseX: ");
    strcat_s(mouseString, tempString);

    result = UpdateSentence(sentence0, mouseString, 20, -20, 1.0f, 1.0f, 1.0f, ctxt);
    if (!result)
        return false;

    _itoa_s(mouseY, tempString, 10);

    strcpy_s(mouseString, "MouseY: ");
    strcat_s(mouseString, tempString);

    result = UpdateSentence(sentence1, mouseString, 20, -40, 1.0f, 1.0f, 1.0f, ctxt);
    if (!result)
        return false;

    return true;
}

bool Text::InitSentence(Sentence** sentence, int maxLength, ID3D11Device* device)
{
    Vertex* vertices;
    unsigned long* indecies;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    *sentence = new Sentence;
    if(!*sentence)
    {
        return false;
    }
    (*sentence)->vertexBuffer = 0;
    (*sentence)->indexBuffer = 0;
    (*sentence)->maxLength = maxLength;
    (*sentence)->vertexCount = 6 * maxLength;
    (*sentence)->indexCount =(*sentence)->vertexCount;
    vertices = new Vertex[(*sentence)->vertexCount];
    if (!vertices)
        return false;
    indecies = new unsigned long[(*sentence)->indexCount];
    if (!indecies)
        return false;
    memset(vertices, 0, (sizeof(Vertex) * (*sentence)->vertexCount));

    for (int i = 0; i < (*sentence)->indexCount; i++)
    {
        indecies[i] = i;
    }

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = 0x2L;//0x2L = bind index buffer
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indecies;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
    if (FAILED(result))
        return false;


    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = 0x1L; // 0x1L = bind vertex buffer
    vertexBufferDesc.CPUAccessFlags = 0x10000L;//0x10000L = cpu acces write
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
    if (FAILED(result))
        return false;

    delete[] vertices;
    vertices = 0;
    delete[] indecies;
    indecies = 0;

    return true;
}

bool Text::UpdateSentence(Sentence* sentence, char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* ctxt)
{
    int numLeters;
    Vertex* vertices;
    float drawX, drawY;
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    Vertex* vertexPtr;

    sentence->red = r;
    sentence->blue = b;
    sentence->green = g;

    numLeters = (int)strlen(text);

    if (numLeters > sentence->maxLength)
        return false;
    vertices = new Vertex[sentence->vertexCount];
    if (!vertices)
        return false;

    memset(vertices, 0, (sizeof(Vertex) * sentence->vertexCount));
    drawX = (float)(((m_screenW / 2) * -1) + posX);
    drawY = (float)(((m_screenH / 2) * -1) - posY);

    m_Font->BuidVertexarray((void*)vertices, text, drawX, drawY);
    result = ctxt->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;
    vertexPtr = (Vertex*)mappedResource.pData;
    
    memcpy(vertexPtr, (void*)vertices, (sizeof(Vertex) * sentence->vertexCount));
    ctxt->Unmap(sentence->vertexBuffer, 0);

    delete[] vertices;
    vertices = 0;

    return true;
}

void Text::Releasesentence(Sentence** sentence)
{
    if(*sentence)
    {
        if((*sentence)->vertexBuffer)
        {
            (*sentence)->vertexBuffer->Release();
            (*sentence)->vertexBuffer = 0;
        }
        if((*sentence)->indexBuffer)
        {
            (*sentence)->indexBuffer->Release();
            (*sentence)->indexBuffer = 0;
        }
        delete (*sentence);
        (*sentence) = 0;

    }
    return;
}

bool Text::RenderSentence(ID3D11DeviceContext* ctxt, Sentence* sentence, XMMATRIX world, XMMATRIX ortho)
{
    unsigned int stride, offset;
    XMFLOAT4 pixelColor;
    bool result;

    stride = sizeof(Vertex);
    offset = 0;

    ctxt->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
    ctxt->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT,0);
    ctxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);
    result = m_FontShader->Render(ctxt, sentence->indexCount, world, m_baseViewMat, ortho, m_Font->GetTexture(), pixelColor);
    if (!result)
        return false;
    return true;
}
