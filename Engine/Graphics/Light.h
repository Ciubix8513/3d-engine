#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_
#pragma region includes
#include "../System/Math/EngineMath.h"

using namespace EngineMath;
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
	Vector4 GetSpecularColor();
	float GetSpecularPower();
	void SetSpecularPower(float);
	void SetDirection(float, float, float);
	Vector4 GetAmbientColor();
	Vector4 GetDiffuseColor();
	Vector3 GetDirection();
#pragma endregion
#pragma region  private vars
private:
	float m_specularPower;
	Vector4 m_specularColor;
	Vector4 m_ambientColor;
	Vector3 m_direction;
	Vector4 m_diffuseColor;
#pragma endregion


};

#endif