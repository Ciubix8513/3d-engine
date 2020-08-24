#include "Texture.h"

Texture::Texture()
{
    m_texture = 0;
    m_textureView = 0;
    m_targaData = 0;
}

Texture::Texture(const Texture&)
{
}

Texture::~Texture()
{
}

bool Texture::Init(ID3D11Device* device, ID3D11DeviceContext* ctxt, char* file)
{
    bool result;
    int height, width;
    D3D11_TEXTURE2D_DESC desc;
    HRESULT Hresult;
    unsigned int rowPitch;
    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    result = loadTarga(file, height, width);
    if (!result) 
    {
        std::cout << file << " does not exist" << std::endl;
        return false;
    }
    desc.Height = height;
    desc.Width = width;
    desc.MipLevels = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.ArraySize = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    Hresult = device->CreateTexture2D(&desc,NULL,&m_texture);
    if (FAILED(Hresult))
        return false;
    rowPitch = (width * 4) * sizeof(unsigned char);
    ctxt->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    Hresult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
    if (FAILED(Hresult))
        return false;

    ctxt->GenerateMips(m_textureView);
    delete [] m_targaData;
    m_targaData = 0;


    return true;
}

void Texture::Shutdown()
{
    if (m_textureView)
    {
        m_textureView->Release();
        m_textureView = 0;
    }
    if (m_targaData)
    {
        delete[] m_targaData;
        m_targaData = 0;
    }
    if (m_texture)
    {
        m_texture->Release();
        m_texture = 0;
    }
    return;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
    return m_textureView;
}

bool Texture::loadTarga(char* file, int& height, int& width)
{
    int error, bpp, imageSize, index;
    FILE* filePtr;
    unsigned int count;
    TargaHeader FileHeader;
    unsigned char* targaImage;

    error = fopen_s(&filePtr, file, "rb");
    if (error != 0)
        return false;
    count = (unsigned int)fread(&FileHeader, sizeof(TargaHeader), 1, filePtr);
    if(count != 1)
        return false;
    height = (int)FileHeader.height;
    width = (int)FileHeader.width;
    bpp = (int)FileHeader.bpp;
    if(bpp != 32)
        return false;
    imageSize = width * height * 4;
    targaImage = new unsigned char[imageSize];

    if (!targaImage)
        return false;

    count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
    if (count != imageSize)
        return false;
    error = fclose(filePtr);
    if(error != 0)
        return false;
    m_targaData = new unsigned char[imageSize];
    if (!m_targaData)
        return false;
    index = 0;
    int k = (width * height * 4) - (width * 4);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            m_targaData[index + 0] = targaImage[k + 2];//r
            m_targaData[index + 1] = targaImage[k + 1];//g
            m_targaData[index + 2] = targaImage[k + 0];//b
            m_targaData[index + 3] = targaImage[k + 3];//a
            k += 4;
            index += 4;
        }
        k -= (width * 8);
    }

    delete[] targaImage;
    targaImage = 0;
    return true;
}
