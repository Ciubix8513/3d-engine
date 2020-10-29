#include "Transform.h"

Engine::Transform::Transform()
{
}

Matrix4x4 Engine::Transform::GetTransformationMatrix()
{
    return TransformationMatrix(Position,Rotation.Euler(),Scale);
}

void Engine::Transform::SetPosition(Vector3 position)
{
    Position = position;
}

void Engine::Transform::SetPosition(float position)
{
    Position = Vector3(position, position, position);
}

void Engine::Transform::SetRotation(Vector3 roation)
{
    Rotation = Quaternion(roation);
}

void Engine::Transform::SetRotation(Quaternion rotation)
{
    Rotation = rotation;
}

void Engine::Transform::SetScale(Vector3 scale)
{
    Scale = scale;
}

void Engine::Transform::SetScale(float scale)
{
    Scale = Vector3(scale, scale, scale);

}
