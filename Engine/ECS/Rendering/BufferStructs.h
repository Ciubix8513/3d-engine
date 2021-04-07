#ifndef _BUFFER_STRUCTS_H_
#define _BUFFER_STRUCTS_H_
#include "..\..\System\Math\EngineMath.h"
using namespace EngineMath;

namespace Engine
{
	class BufferClass
	{
	
	};	

	class MatrixBuffer : public BufferClass
	{
	public:
		MatrixBuffer();
		~MatrixBuffer();

	private:
		Matrix4x4
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			objectMat;
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

