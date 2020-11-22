#include "Transform.h"



Engine::Transform::Transform()
{
}

Matrix4x4 Engine::Transform::GetTransformationMatrix()
{
    return TransformationMatrix(Position,Rotation.Euler(),Scale);
}
void Engine::Transform::SetChild(Transform** Child)
{
    m_Children.push_back(Child);
}
void Engine::Transform::SetParrent(Transform** Parent)
{
    m_Parent = Parent;

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


Vector3 Engine::Transform::GetReletivePosition()
{
    return Position;
}
Vector3 Engine::Transform::GetAbsolutePosition()
{
    if(HasParent())    
        return (*m_Parent)->GetAbsolutePosition() + Position;    
    return Position;
}
Vector3 Engine::Transform::GetReletiveScale()
{
    return Scale;
}
Vector3 Engine::Transform::GetAbsoluteScale()
{
    if (HasParent()) 
    {
        Vector3 a = (*m_Parent)->GetAbsoluteScale();
        return Vector3(a.x * Scale.x, a.y * Scale.y, a.z * Scale.z);
    }
    return Scale;
}
Quaternion Engine::Transform::GetReletiveRotation()
{
    return Rotation;
}
Quaternion Engine::Transform::GetAbsoluteRotation()
{
    if (HasParent())
        return (*m_Parent)->GetAbsoluteRotation() * Rotation;
    return Rotation;
}


Engine::Transform** Engine::Transform::GetParent()
{
    return m_Parent;
}

Engine::Transform** Engine::Transform::GetChild(unsigned int ChildIndex)
{
    return m_Children[ChildIndex];
}

Engine::Transform*** Engine::Transform::GetChildren(unsigned int ChildIndex0, unsigned int ChildIndex1)
{
    if (ChildIndex0 > ChildIndex1)
        throw exception("ChildIndex0 must be less then ChildIndex1");
    Transform*** a = new Transform * *[ChildIndex1 - ChildIndex0];
    for (size_t i = 0; i < ChildIndex1 - ChildIndex0; i++)
    {
        a[i] = m_Children[i];
    }
    return a;
}

Engine::Transform*** Engine::Transform::GetChildren(unsigned int* ChildrenIndecies, unsigned int ChildrenCount)
{
    auto a = new Transform * *[ChildrenCount];
    for (size_t i = 0; i < ChildrenCount; i++)
    {
        a[i] = m_Children[ChildrenIndecies[i]];
    }

    return a;
}



bool Engine::Transform::HasParent()
{
    return HasPar;
}

bool Engine::Transform::HasChildren()
{
    return m_Children.size() != 0;
}

unsigned int Engine::Transform::GetChildrenCount()
{
    return m_Children.size();    
}

