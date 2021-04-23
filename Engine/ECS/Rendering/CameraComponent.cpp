#include "CameraComponent.h"

std::vector<const type_info*> Engine::CameraComponent::GetRequieredComponents()
{
	return {&typeid(Transform)};
}

void Engine::CameraComponent::Initialise(std::vector<Component*> Components, D3d** d3d)
{
	m_Transform = (Transform*)Components[0];

	return;
}

void Engine::CameraComponent::Update()
{

	m_viewMat = EngineMath::LookAtMatrix(m_Transform->GetAbsolutePosition(), m_Transform->RotateVectorAbs(Vector3(0, 0, 1)), m_Transform->RotateVectorAbs(Vector3(0, 1, 0)));
}

Matrix4x4 Engine::CameraComponent::GetProjectionMatrix()
{
	return m_projMat;
}

Matrix4x4 Engine::CameraComponent::GetViewMatrix()
{
	return m_viewMat;
}

void Engine::CameraComponent::SetCameraParams(CameraType type, float fov, float near, float far, int screenWidth, int screenHeight)
{
	m_Type = type;
	m_FOV = fov;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_ScreenAspect = (float)m_screenWidth / (float)m_screenHeight;
	m_ScreenFar = far;
	m_ScreenNear = near;

	if (m_Type == Perspective)
		m_projMat = PerspectiveProjectionMatrix(m_FOV, m_ScreenAspect, m_ScreenNear, m_ScreenFar);
	else
		//throw std::exception("Not yet implemented"); 
		m_projMat = OrthographicProjectionMatrix(m_screenWidth, m_screenHeight, m_ScreenNear, m_ScreenFar);
	return;
	

}

//IDK what they're needed for but I'll leave 'em just in case
#define near 
#define far