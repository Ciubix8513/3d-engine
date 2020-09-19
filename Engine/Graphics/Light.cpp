#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

Light::Light(const Light&)
{
}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = Vector4(r, g, b, a);
	return;
}

void Light::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = Vector4(r, g, b, a);
	return;
}

void Light::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = Vector4(r, g, b, a);
	return;

}

Vector4 Light::GetSpecularColor()
{
	return m_specularColor;
}

float Light::GetSpecularPower()
{
	return m_specularPower;
}

void Light::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = Vector3(x, y, z);
	return;
}

Vector4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

Vector4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

Vector3 Light::GetDirection()
{
	return m_direction;
}
