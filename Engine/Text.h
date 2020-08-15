#pragma once
#ifndef _TEXT_H_
#define _TEXT_H_
#pragma region includes
#include "Font.h"
#include "../Engine/ShaderClasses/FontShader.h"
#pragma endregion

class Text
{
#pragma region structs
struct Sentence
{
	ID3D11Buffer* vertexBuffer, * indexBuffer;
	int vertexCount, indexCount, maxLength;
	float red, green, blue;
};
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 UV;
};
#pragma endregion
#pragma region public funcs
public:
Text();
Text(const Text&);
~Text();
bool Init(ID3D11Device* device, ID3D11DeviceContext* ctxt, HWND hwnd, int scrW, int scrH, XMMATRIX viewMat, int numChars);
void ShutDown();
bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX);
bool SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* ctxt);

#pragma endregion
#pragma region private funcs
bool InitSentence(Sentence**, int, ID3D11Device*);
bool UpdateSentence(Sentence*, char*, int, int, float, float, float, ID3D11DeviceContext*);
void Releasesentence(Sentence**);
bool RenderSentence(ID3D11DeviceContext*, Sentence*, XMMATRIX, XMMATRIX);
#pragma endregion
#pragma region private vars
private:
	Font* m_Font;
	FontShader* m_FontShader;
	int m_screenW;
	int m_screenH;
	XMMATRIX m_baseViewMat;
	
	Sentence* sentence0; 
	Sentence* sentence1;
#pragma endregion



};

#endif // !_TEXT_H_
