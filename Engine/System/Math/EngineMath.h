#pragma once
#ifndef _ENGINE_MATH_H_
#define _ENGINE_MATH_H_
#include "Vectors/Plane.h"
using namespace EngineMath;

namespace EngineMath 
{
#define PI (double)3.1415926535897932384626433832795f
	
	extern Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b);
	extern Vector4 TransformVector(Vector4 v, Matrix4x4 m);
	extern Matrix4x4 PerspectiveProjectionMatrix(float FOV, float  screenAspect, float  screenNear, float  screenDepth);
	extern Matrix4x4 OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear,float screenDepth);
	extern Matrix4x4 Transpose(Matrix4x4 m);
	extern Matrix4x4 Identity();
	extern Matrix4x4 LookAtMatrix(Vector3 CameraPosition, Vector3 CameraForward, Vector3 cameraUp);
	extern Matrix4x4 RotationPitchYawRoll(float pitch, float yaw, float roll);
	extern Matrix4x4 RotationPitchYawRoll(Vector3 rpy);
	extern Matrix4x4 GetTramsformationMatrix(Vector3 posistion, Vector3 Rotation, Vector3 Scale);
#pragma region plain funcs
	extern float PlaneDotCoord(Plane plane, Vector3 point);
	extern Vector3 Normalized(Vector3 v);

#pragma endregion

};
#endif
