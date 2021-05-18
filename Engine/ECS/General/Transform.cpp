#include "Transform.h"

void Engine::Transform::Initialise(std::vector<Component*> Comps, D3d** d3d, ULONG entityUUID)
{
    Position = Vector3(0,0,0);
    Scale = Vector3(1, 1, 1);
    Rotation = Vector4(0, 0, 0, 0);
    EntityUUID = entityUUID;
}

std::string Engine::Transform::GetName()
{
    return "Transform";
}

//void Engine::Transform::Serialise(std::string Fname)
//{
//    std::ofstream f(Fname);
//    f << (void*)this;
//   f.close();
//}

Engine::Transform::Transform()
{
}
#pragma region Set transformation funcs
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
#pragma endregion
#pragma region Get transformation funcs
Matrix4x4 Engine::Transform::GetTransformationMatrix()
{
    return TransformationMatrix(Position, Rotation.Euler(), Scale);
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

Vector3 Engine::Transform::RotateVectorAbs(Vector3 vector)
{
    return  (EngineMath::RotationPitchYawRoll( GetAbsoluteRotation().Euler()) *  Vector4(vector,0)).XYZ();     
}
Vector3 Engine::Transform::RotateVectorRel(Vector3 vector)
{
    return  (EngineMath::RotationPitchYawRoll(GetReletiveRotation().Euler()) * Vector4(vector, 0)).XYZ();
}

Vector4 Engine::Transform::RotateVectorAbs(Vector4 vector)
{
    return  (EngineMath::RotationPitchYawRoll(GetAbsoluteRotation().Euler()) * vector);
}
Vector4 Engine::Transform::RotateVectorRel(Vector4 vector)
{
    return  (EngineMath::RotationPitchYawRoll(GetReletiveRotation().Euler()) * vector);
}
Vector2 Engine::Transform::RotateVectorAbs(Vector2 vector)
{
    return  (EngineMath::RotationPitchYawRoll(GetAbsoluteRotation().Euler()) * Vector4(vector, 0,0)).XY();
}
Vector2 Engine::Transform::RotateVectorRel(Vector2 vector)
{
    return  (EngineMath::RotationPitchYawRoll(GetReletiveRotation().Euler()) * Vector4(vector, 0, 0)).XY();
}


#pragma endregion
#pragma region ChildParentStuff
void Engine::Transform::SetChild(Transform** Child)
{
    m_Children.push_back(Child);
    auto a = this;
    (*Child)->HasPar = true;
    (*Child)->m_Parent = &a;
}
void Engine::Transform::SetParrent(Transform** Parent)
{
    auto a = this;
    (*Parent)->SetChild(&a);
}
Engine::Transform** Engine::Transform::GetParent()
{
    return m_Parent;
}
Engine::Transform** Engine::Transform::GetChild(unsigned int ChildIndex)
{
    if (ChildIndex < m_Children.size())
        return m_Children[ChildIndex];
    else
        throw std::exception("wrong child index");
    return nullptr;


}
Engine::Transform*** Engine::Transform::GetChildren(unsigned int ChildIndex0, unsigned int ChildIndex1)
{
    if (ChildIndex0 > ChildIndex1) 
    {    
        throw std::exception("ChildIndex0 must be less then ChildIndex1");
        return nullptr;
    }
    if(ChildIndex0 >  m_Children.size())
    {
        throw std::exception("wrong child index 0");
        return nullptr;
    }
    if (ChildIndex1 >= m_Children.size())
    {
        throw std::exception("wrong child index 1");
        return nullptr;
    }



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
        if(ChildrenIndecies[i] >= m_Children.size())
        {
            throw std::exception("wrong child index " + i);
            return nullptr;
        }
        a[i] = m_Children[ChildrenIndecies[i]];
    }

    return a;
}
Engine::Transform*** Engine::Transform::GetChildren(std::vector<unsigned int> ChildrenIndecies)
{
    auto a = new Transform * *[ChildrenIndecies.size()];
    for (size_t i = 0; i < ChildrenIndecies.size(); i++)
    {
        if (ChildrenIndecies[i] >=m_Children.size())
        {
            throw std::exception("wrong child index " + i);
            return nullptr;
        }
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
void Engine::Transform::RemoveParent()
{  
    auto tmp = this;
    (*m_Parent)->RemoveChild(&tmp);
    m_Parent = nullptr;   
}
void Engine::Transform::RemoveChild(Transform** Child)
{
    for (auto i = m_Children.begin(); i != m_Children.end(); i++)
    {
        if ((*i) == Child)
        {
            (*Child)->HasPar = false;
            m_Children.erase(i);
            return;
        }
    }
}
void Engine::Transform::RemoveChild(unsigned int ChildIndex)
{
    if(ChildIndex >= m_Children.size())
    {
        throw std::exception("wrong child index");
        return ;
    }
    (*m_Children[ChildIndex])->HasPar = false;
    auto i = m_Children.begin() + ChildIndex;
    m_Children.erase(i);
}
void Engine::Transform::RemoveChildren(Transform*** Children, unsigned int ChildrenCount)
{
    for (size_t i = 0; i < ChildrenCount; i++)    
        RemoveChild(Children[i]); // Should work, but may be slow    
}
void Engine::Transform::RemoveChildren(std::vector<Transform**> Children)
{
    for (size_t i = 0; i < Children.size(); i++)    
        RemoveChild(Children[i]); // Should work, but may be slow    
}
void Engine::Transform::RemoveChildren(unsigned int* ChildrenIndecies, unsigned int ChildrenCount)
{
    for (size_t i = 0; i < ChildrenCount; i++)
        RemoveChild(ChildrenIndecies[i]);
}
void Engine::Transform::RemoveChildren(unsigned int ChildIndex0, unsigned int ChildIndex1)
{
    if (ChildIndex0 > ChildIndex1)
    {
        throw std::exception("ChildIndex0 must be less then ChildIndex1");
        return;
    }
    for (size_t i = 0; i < ChildIndex1 - ChildIndex0; i++)
        RemoveChild(i);
}
void Engine::Transform::RemoveChildren(std::vector<unsigned int> ChildrenIndecies)
{
    for (size_t i = 0; i < ChildrenIndecies.size(); i++)
        RemoveChild(ChildrenIndecies[i]);   
}
void Engine::Transform::RemoveAllChildren()
{   
    for (size_t i = 0; i < m_Children.size(); i++)    
        RemoveChild(i);       
}
#pragma endregion

