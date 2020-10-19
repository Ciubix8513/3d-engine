#include "Quarernion.h"

Quaternion::Quaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
Quaternion::~Quaternion()
{
}
Quaternion::Quaternion(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}
Quaternion::Quaternion(Vector4 xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
	w = xyzw.w;
}
Quaternion::Quaternion(Vector3 xyz, float W)
{

	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
	w = W;
}
Quaternion::Quaternion(Vector2 xy, Vector2 zw)
{
	x = xy.x;
	y = xy.y;
	z = zw.x;
	w = zw.y;
}
