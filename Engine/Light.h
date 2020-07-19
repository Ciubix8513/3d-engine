#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_
#pragma region includes
#include <DirectXMath.h>
using namespace DirectX;
#pragma endregion


class Light
{
#pragma region public funcs
public:
	Light();
	~Light();
	Light(const Light&);

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
#pragma endregion
#pragma region  private vars
private:
	XMFLOAT3 m_direction;
	XMFLOAT4 m_diffuseColor;
#pragma endregion


};

#endif