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

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);	
	void SetSpecularColor(float, float, float, float);
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();
	void SetSpecularPower(float);
	void SetDirection(float, float, float);
	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
#pragma endregion
#pragma region  private vars
private:
	float m_specularPower;
	XMFLOAT4 m_specularColor;
	XMFLOAT4 m_ambientColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_diffuseColor;
#pragma endregion


};

#endif