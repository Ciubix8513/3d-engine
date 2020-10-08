#include "FreeMovement.h"

FreeMovement::FreeMovement()
{
}
FreeMovement::FreeMovement(const FreeMovement&)
{
}
FreeMovement::~FreeMovement()
{
}

void FreeMovement::Init()
{
	m_Speed = .01f;
	m_MouseSensetivity = .1f;
	m_Controls[0] = DIK_W;
	m_Controls[1] = DIK_S;
	m_Controls[2] = DIK_A;
	m_Controls[3] = DIK_D;
	m_Controls[4] = DIK_SPACE;
	m_Controls[5] = DIK_LSHIFT;
}

void FreeMovement::SetControls(int Key, size_t keyIndex)
{
	m_Controls[keyIndex] = Key;
}

void FreeMovement::SetMouseSensetivity(float sensetivity)
{
	m_MouseSensetivity = sensetivity;
}

void FreeMovement::SetSpeed(float speed)
{
	m_Speed = speed;
}

void FreeMovement::MoveCamera(Camera* camera, Input* input, float dTime)
{


	
float pitch = camera->m_RotX * 0.0174532925f;
float yaw = camera->m_RotY * 0.0174532925f;
float roll = camera->m_RotZ * 0.0174532925f;

	Matrix4x4 rotMatrix = RotationPitchYawRoll(pitch, yaw, roll);

	if (input->isKeyDown(m_Controls[0]))
	{
		Vector4 Direction = Vector4(0, 0, 1,0);
		Direction = rotMatrix* Direction;
		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}
	if (input->isKeyDown(m_Controls[1]))
	{
		Vector4 Direction = Vector4(0, 0, -1, 0);
		Direction = rotMatrix * Direction;

		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}
	if (input->isKeyDown(m_Controls[2]))
	{
		Vector4 Direction = Vector4(-1, 0, 0, 0);
    	Direction = rotMatrix * Direction;

		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}
	if (input->isKeyDown(m_Controls[3]))
	{
		Vector4 Direction = Vector4(1, 0, 0, 0);
		Direction = rotMatrix * Direction;

		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}
	if (input->isKeyDown(m_Controls[4]))
	{
		Vector4 Direction = Vector4(0, 1, 0, 0);
		Direction = rotMatrix * Direction;

		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}
	if (input->isKeyDown(m_Controls[5]))
	{
		Vector4 Direction = Vector4(0, -1, 0, 0);
		Direction = rotMatrix * Direction;
		camera->SetPosition(Vector3(camera->GetPosition().x + (Direction.x * m_Speed * dTime), camera->GetPosition().y + (Direction.y * m_Speed * dTime), camera->GetPosition().z + (Direction.z * m_Speed * dTime)));
	}

	int Dx, Dy;
	input->GetMouseDelta(Dx, Dy);

	float newRotationX = camera->GetRotation().y + Dx * m_MouseSensetivity;
	float newRotationY = camera->GetRotation().x + Dy * m_MouseSensetivity;
	camera->SetRotation(newRotationY, newRotationX, 0);

	return;
}