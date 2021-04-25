#include "BufferStructs.h"
	

Engine::MatrixBuffer::MatrixBuffer(Matrix4x4 WorldMatrix, Matrix4x4 ViewMatrix, Matrix4x4 ProjectionMatrix)
{
	worldMatrix = WorldMatrix;
	viewMatrix = ViewMatrix;
	projectionMatrix = ProjectionMatrix;
};