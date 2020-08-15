#include "Font.h"

Font::Font()
{
    m_Font = 0;
    m_Texture = 0;
}

Font::Font(const Font&)
{
}

Font::~Font()
{
}

bool Font::Init(ID3D11Device* device,ID3D11DeviceContext* ctxt, char* fontFile, char* fontTexture,int numChars)
{
    bool result;
    
    result = LoadFontData(fontFile,numChars);
    if (!result) 
    {
        cout << "Could not load font file" << endl;

        return false;
    }
    result = LoadTexture(device, ctxt, fontTexture);
    if (!result) 
    {
        cout << "Could not load font texture" << endl;

        return false;
    }
    return true;
}

void Font::ShutDown()
{
    ReleaseTexture();
    ReleaseFontData();
    return;
}

ID3D11ShaderResourceView* Font::GetTexture()
{
    return m_Texture->GetTexture();
}

void Font::BuidVertexarray(void* vertices, char* sentence, float drawX, float drawY)
{
    Vertex* vertexPtr;
    int numletters,index,letter;
    
    vertexPtr = (Vertex*)vertices;

    numletters = (int)strlen(sentence);

    index = 0;

    for (int i = 0; i < numletters; i++)
    {
        letter = ((int)sentence[i]) - 32;
        
        if (letter == 0)
        {
            drawX += 3.0f;
        }
        else
        {
            //triangle 0
            // First triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;
            
            vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].left, 1.0f);
            index++;

            // Second triangle in quad.
            vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].left, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].right, 0.0f);
            index++;

            vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
            vertexPtr[index].Texture = XMFLOAT2(m_Font[letter].right, 1.0f);
            index++;

            // Update the x location for drawing by the size of the letter and one pixel.
            drawX = drawX + m_Font[letter].size + 1.0f;
        }
    }
    return;
}

bool Font::LoadFontData(char* file, int numChars)
{
    ifstream fin;
    char tmp;
    
    m_Font = new FontType[numChars];
    if (!m_Font)
        return false;
    
    fin.open(file);
    if (fin.fail())
        return false;

    for (int i = 0; i < numChars; i++)
    {
        fin.get(tmp);
        while (tmp != ' ')
        {
            fin.get(tmp);
        }
        fin.get(tmp);
        while (tmp != ' ')
        {
            fin.get(tmp);
        }

        fin >> m_Font[i].left;
        fin >> m_Font[i].right;
        fin >> m_Font[i].size;
    }


    fin.close();

    return true;
}

void Font::ReleaseFontData()
{
    if (m_Font)
    {
        delete[] m_Font;
        m_Font = 0;
    }
    return;
}

bool Font::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* ctxt, char* file)
{
    bool result;

    m_Texture = new Texture;
    if (!m_Texture)
        return false;

    result = m_Texture->Init(device, ctxt, file);
    if (!result)
        return false;
    return true;
}

void Font::ReleaseTexture()
{
    if (m_Texture)
    {
        m_Texture->Shutdown();
        m_Texture = 0;
    }
}
