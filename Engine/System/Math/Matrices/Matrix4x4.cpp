#include "Matrix4x4.h"


EngineMath::Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    _m00 = m00;
    _m01 = m01;
    _m02 = m02;
    _m03 = m03;

    _m10 = m10;
    _m11 = m11;
    _m12 = m12;
    _m13 = m13;

    _m20 = m10;
    _m21 = m11;
    _m22 = m12;
    _m23 = m13;

    _m30 = m10;
    _m31 = m11;
    _m32 = m12;
    _m33 = m13;
}
EngineMath::Matrix4x4::Matrix4x4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
{
    SetRow(0, row0);
    SetRow(1, row1);
    SetRow(2, row2);
    SetRow(3, row3);
}
EngineMath::Matrix4x4::Matrix4x4()
{
    
    SetRow(0, Vector4(1, 0, 0, 0));
    SetRow(1, Vector4(0, 1, 0, 0));
    SetRow(2, Vector4(0, 0, 1, 0));
    SetRow(3, Vector4(0, 0, 0, 1));

}

Matrix4x4 EngineMath::Matrix4x4::Transposed()
{

    Matrix4x4 m;
    for (int i = 0; i < 4; i++)
        m.SetRow(i, GetRow(i));
    Matrix4x4 Out;

    Out._m00 = m._m00;
    Out._m01 = m._m10;
    Out._m02 = m._m20;
    Out._m03 = m._m30;

    Out._m11 = m._m11;
    Out._m10 = m._m01;
    Out._m12 = m._m21;
    Out._m13 = m._m31;

    Out._m22 = m._m22;
    Out._m21 = m._m12;
    Out._m20 = m._m02;
    Out._m23 = m._m32;

    Out._m33 = m._m33;
    Out._m31 = m._m13;
    Out._m32 = m._m23;
    Out._m03 = m._m30;

    return Out;
}
void EngineMath::Matrix4x4::Transpose()
{
    Matrix4x4 m ;
    for (int i = 0; i < 4; i++)    
        m.SetRow(i,GetRow(i));
    

    _m01 = m._m10;
    _m02 = m._m20;
    _m03 = m._m30;

    _m10 = m._m01;
    _m12 = m._m21;
    _m13 = m._m31;

    _m21 = m._m12;
    _m20 = m._m02;
    _m23 = m._m32;

    _m31 = m._m13;
    _m32 = m._m23;
    _m03 = m._m30;  
}

void EngineMath::Matrix4x4::SetRow(int row, Vector4 value)
{
    switch (row)
    {
    case 0:
        _m00 = value.x;
        _m01 = value.y;
        _m02 = value.z;
        _m03 = value.w;
        break;
    case 1:
        _m10 = value.x;
        _m11 = value.y;
        _m12 = value.z;
        _m13 = value.w;
        break;
    case 2:
        _m20 = value.x;
        _m21 = value.y;
        _m22 = value.z;
        _m23 = value.w;
        break;
    case 3:
        _m30 = value.x;
        _m31 = value.y;
        _m32 = value.z;
        _m33 = value.w;
        break;
    default:
        throw exception("Wrong row index");
        break;
    }
    return;

}
Vector4 EngineMath::Matrix4x4::TransformVector(Vector4 v)
{
    Vector4 out;
    out.x = v.x * _m00 + v.y * _m01 + v.z * _m02 + v.w * _m03;
    out.y = v.x * _m10 + v.y * _m11 + v.z * _m12 + v.w * _m13;
    out.z = v.x * _m20 + v.y * _m21 + v.z * _m22 + v.w * _m23;
    out.w = v.x * _m30 + v.y * _m31 + v.z * _m32 + v.w * _m33;

    return out;
}

Matrix4x4 EngineMath::Matrix4x4::operator+(Matrix4x4 m)
{
    return Matrix4x4(GetRow(0)+ m.GetRow(0), GetRow(1) + m.GetRow(1), GetRow(2) + m.GetRow(2), GetRow(3) + m.GetRow(3));
}
Matrix4x4 EngineMath::Matrix4x4::operator-(Matrix4x4 m)
{
    return Matrix4x4(GetRow(0) - m.GetRow(0), GetRow(1) - m.GetRow(1), GetRow(2) - m.GetRow(2), GetRow(3) - m.GetRow(3));

}
Matrix4x4 EngineMath::Matrix4x4::operator*(float c)
{
    return Matrix4x4(GetRow(0)* c, GetRow(2) * c, GetRow(3) * c, GetRow(4) * c);
}
Matrix4x4 EngineMath::Matrix4x4::operator/(float c)
{
    return Matrix4x4(GetRow(0) / c, GetRow(2) / c, GetRow(3) / c, GetRow(4) / c);
}
Matrix4x4 EngineMath::Matrix4x4::operator*(Matrix4x4 m)
{
    Vector4 Out[4];
    Out[0].x = (_m00 * m._m00) + (_m01 * m._m10) + (_m02 * m._m20) + (_m03 * m._m30);
    Out[0].y = (_m00 * m._m01) + (_m01 * m._m11) + (_m02 * m._m21) + (_m03 * m._m31);
    Out[0].x = (_m00 * m._m02) + (_m01 * m._m12) + (_m02 * m._m22) + (_m03 * m._m32);
    Out[0].x = (_m00 * m._m03) + (_m01 * m._m13) + (_m02 * m._m23) + (_m03 * m._m33);

    Out[1].x = (_m10 * m._m00) + (_m11 * m._m10) + (_m12 * m._m20) + (_m13 * m._m30);
    Out[1].y = (_m10 * m._m01) + (_m11 * m._m11) + (_m12 * m._m21) + (_m13 * m._m31);
    Out[1].x = (_m10 * m._m02) + (_m11 * m._m12) + (_m12 * m._m22) + (_m13 * m._m32);
    Out[1].x = (_m10 * m._m03) + (_m11 * m._m13) + (_m12 * m._m23) + (_m13 * m._m33);

    Out[2].x = (_m20 * m._m00) + (_m21 * m._m10) + (_m22 * m._m20) + (_m23 * m._m30);
    Out[2].y = (_m20 * m._m01) + (_m21 * m._m11) + (_m22 * m._m21) + (_m23 * m._m31);
    Out[2].x = (_m20 * m._m02) + (_m21 * m._m12) + (_m22 * m._m22) + (_m23 * m._m32);
    Out[2].x = (_m20 * m._m03) + (_m21 * m._m13) + (_m22 * m._m23) + (_m23 * m._m33);

    Out[3].x = (_m30 * m._m00) + (_m31 * m._m10) + (_m32 * m._m20) + (_m33 * m._m30);
    Out[3].y = (_m30 * m._m01) + (_m31 * m._m11) + (_m32 * m._m21) + (_m33 * m._m31);
    Out[3].x = (_m30 * m._m02) + (_m31 * m._m12) + (_m32 * m._m22) + (_m33 * m._m32);
    Out[3].x = (_m30 * m._m03) + (_m31 * m._m13) + (_m32 * m._m23) + (_m33 * m._m33);
    return Matrix4x4(Out[0], Out[1], Out[2], Out[3]);
}
void EngineMath::Matrix4x4::operator+=(Matrix4x4 m)
{
    SetRow(0, GetRow(0) + m.GetRow(0));
    SetRow(1, GetRow(1) + m.GetRow(1));
    SetRow(2, GetRow(2) + m.GetRow(2));
    SetRow(3, GetRow(3) + m.GetRow(3));
    return;

}
void EngineMath::Matrix4x4::operator-=(Matrix4x4 m)
{
    SetRow(0, GetRow(0) - m.GetRow(0));
    SetRow(1, GetRow(1) - m.GetRow(1));
    SetRow(2, GetRow(2) - m.GetRow(2));
    SetRow(3, GetRow(3) - m.GetRow(3));
    return;
}
void EngineMath::Matrix4x4::operator*=(float c)
{
    SetRow(0, GetRow(0) * c);
    SetRow(1, GetRow(1) * c);
    SetRow(2, GetRow(2) * c);
    SetRow(3, GetRow(3) * c);
    return;
}
void EngineMath::Matrix4x4::operator/=(float c)
{
    SetRow(0, GetRow(0) / c);
    SetRow(1, GetRow(1) / c);
    SetRow(2, GetRow(2) / c);
    SetRow(3, GetRow(3) / c);
    return;

}
void EngineMath::Matrix4x4::operator=(Matrix4x4 m)
{
    for (size_t i = 0; i < 4; i++)    
        SetRow(i, m.GetRow(i));
    return;
    
}
Vector4 EngineMath::Matrix4x4::operator*(Vector4 v)
{
    Vector4 out;
    out.x = v.x * _m00 + v.y * _m01 + v.z * _m02 + v.w * _m03;
    out.y = v.x * _m10 + v.y * _m11 + v.z * _m12 + v.w * _m13;
    out.z = v.x * _m20 + v.y * _m21 + v.z * _m22 + v.w * _m23;
    out.w = v.x * _m30 + v.y * _m31 + v.z * _m32 + v.w * _m33;

    return out;
}
Vector4 EngineMath::Matrix4x4::operator[](int index)
{
    return GetRow(index);
}

Vector4 EngineMath::Matrix4x4::GetRow(int index)
{
    switch (index)
    {
    case 0:
        return Vector4(_m00, _m01, _m02, _m03);
        break;
    case 1:
        return Vector4(_m10, _m11, _m12, _m13);
        break;
    case 2:
        return Vector4(_m20, _m21, _m22, _m23);
        break;
    case 3:
        return Vector4(_m30, _m31, _m32, _m33);
        break;
    default:
        throw exception("Wrong row index");
        break;
    }
}


