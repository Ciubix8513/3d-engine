#pragma once
#ifndef _TEXT_H_
#define _TEXT_H_
#pragma region includes
#include "Font.h"
#include "../ShaderClasses/FontShader.h"
#pragma endregion

class Text
{
#pragma region structs
public:
struct Sentence
{
	ID3D11Buffer* vertexBuffer, * indexBuffer;
	int vertexCount, indexCount, maxLength;
	float red, green, blue;
};
private:
struct Vertex
{
	Vector3 position;
	Vector2 UV;
};
#pragma endregion
#pragma region public funcs
public:
Text();
Text(const Text&);
~Text();
bool Init(ID3D11Device* device, ID3D11DeviceContext* ctxt, HWND hwnd, int scrW, int scrH, Matrix4x4 viewMat);
bool UpdateSentence(Sentence* sentence, char* text, int posX, int posY, float r, float g, float b, ID3D11DeviceContext* ctxt);
bool InitSentence(Sentence** sentence, int maxLength, ID3D11Device* device);
static void Releasesentence(Sentence**);
bool RenderSentence(ID3D11DeviceContext* ctxt, Sentence* sentence, Matrix4x4 world, Matrix4x4 ortho);
void ShutDown();

#pragma endregion
#pragma region private funcs
#pragma endregion
#pragma region private vars
private:
	Font* m_Font;
	FontShader* m_FontShader;
	int m_screenW;
	int m_screenH;
	Matrix4x4 m_baseViewMat;

#pragma endregion



};

#endif // !_TEXT_H_
