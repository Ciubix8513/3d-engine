#pragma once
#include "Vectors.h"
namespace Engine
{
	
	struct Matrix
	{
		union
		{

		  float m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33;

			  Vector4 r[4];
			  float m[4][4]
		};
	};

}