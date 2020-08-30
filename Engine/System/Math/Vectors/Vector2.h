#pragma once
#ifndef _VECTOR2_H_
#define _VECTOR2_H_
#include <math.h>
#include <exception>
using namespace std;
using namespace EngineMath;

namespace EngineMath
{
	static class Vector2
	{
	public:
#pragma region VectorCreation + Consts
		Vector2(float NewX, float NewY);
		static Vector2 Up();
		static Vector2 Down();
		static Vector2 Right();
		static Vector2 Left();
		static Vector2 Zero();
#pragma endregion




#pragma region Functions
		float Length();
		static float Length(Vector2& v);
		Vector2 Normalized();
		static void Normalize(Vector2& v);
		void Normalize();
		static float DotProduct(Vector2 a, Vector2 b);
		
#pragma endregion

#pragma region operators
		Vector2 operator+(Vector2 v);
		Vector2 operator-(Vector2 v);
		Vector2 operator*(float c);
		Vector2 operator/(float c);
		void operator+=(Vector2 v);
		void operator-=(Vector2 v);
		void operator*=(float c);
		void operator/=(float c);
#pragma endregion
		float x,y;	
	};
};
#endif 
