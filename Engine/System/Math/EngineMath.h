#pragma once
#ifndef _ENGINE_MATH_H_
#define _ENGINE_MATH_H_
#include "Matrices/Matrix4x4.h"
using namespace EngineMath;

namespace EngineMath 
{
#define PI (double)3.1415926535897932384626433832795f
	
	static Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b);
	static Vector4 TransformVector(Vector4 v, Matrix4x4 m);
	static Matrix4x4 PerspectiveProjectionMatrix(float FOV, float  screenAspect, float  screenNear, float  screenDepth);
	static Matrix4x4 OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear,float screenDepth);
	static Matrix4x4 Transpose(Matrix4x4 m);
	static Matrix4x4 Identity();
	static Matrix4x4 LookAtMatrix(Vector3 CameraPosition, Vector3 CameraForward, Vector3 cameraUp);

	static Matrix4x4 RotationPitchYawRoll(float pitch, float yaw, float roll);
};
#endif
