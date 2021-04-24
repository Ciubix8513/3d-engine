#ifndef _BUFFER_STRUCTS_H_
#define _BUFFER_STRUCTS_H_
#include "..\..\System\Math\EngineMath.h"
using namespace EngineMath;

namespace Engine
{
	

	
	class MatrixBuffer 
	{
	public:
		MatrixBuffer(Matrix4x4 WorldMatrix, Matrix4x4 ViewMatrix, Matrix4x4 ProjectionMatrix)
		{
			worldMatrix = WorldMatrix;
			viewMatrix = ViewMatrix;
			projectionMatrix = ProjectionMatrix;
			

		};
		~MatrixBuffer();	
		
		Matrix4x4
			worldMatrix,
			viewMatrix,
			projectionMatrix;
	};

	class CameraBuffer
	{
	public:
		CameraBuffer();
		~CameraBuffer();

	private:
		Vector3 postion;
		float padding;
	};



}

#endif // !_BUFFER_STRUCTS_H_

