#ifndef _FREE_MOVEMENT_H_
#define _FREE_MOVEMENT_H_
#include "../Graphics/Camera/Camera.h"
#include "../Input/Input.h"
#include <iostream>
class FreeMovement
{

public:
	FreeMovement();
	FreeMovement(const FreeMovement&);
	~FreeMovement();
	void MoveCamera(Camera* camera,Input* input,float dTime);
	void Init();
	void SetControls(int Key, size_t keyIndex);
	void SetMouseSensetivity(float sensetivity);
	void SetSpeed(float speed);
#pragma region PrivateVars
private: 
	int m_Controls[6];
	float m_MouseSensetivity;
	float m_Speed;
#pragma endregion


};
#endif
