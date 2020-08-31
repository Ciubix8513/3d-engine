#include "EngineMath.h"

Matrix4x4 EngineMath::Multiply(Matrix4x4 a, Matrix4x4 b)
{
    return a * b;
}

Vector4 EngineMath::TransformVector(Vector4 v, Matrix4x4 m)
{
    return  m * v;
}

Matrix4x4 EngineMath::PerspectiveProjectionMatrix(float FOV, float screenAspect, float screenNear, float screenDepth)
{
    float A, B, C, D, E;
    B = 1 / tan(FOV * .5f);
    A = B * screenAspect;
    C = screenDepth / (screenDepth - screenNear);
    D = 1;
    E = -screenNear * C;


    return Matrix4x4(
        Vector4(A, 0, 0, 0),
        Vector4(0, B, 0, 0),
        Vector4(0, 0, C, D),
        Vector4(0, 0, E, 0));
}

Matrix4x4 EngineMath::OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
    float A, B, C, D, E;
    A = 2 / screenWidth;
    B = 2 / screenHeight;
    C = 1 / (screenDepth - screenNear);
    D = 1;
    E = -screenNear / (screenDepth - screenNear);


    return Matrix4x4(
        Vector4(A, 0, 0, 0),
        Vector4(0, B, 0, 0),
        Vector4(0, 0, C, 0),
        Vector4(0, 0, E, D));


}

Matrix4x4 EngineMath::Identity()
{
    return Matrix4x4(
        Vector4(1, 0, 0, 0),
        Vector4(0, 1, 0, 0),
        Vector4(0, 0, 1, 0),
        Vector4(0, 0, 0, 1));
}

Matrix4x4 EngineMath::Transpose(Matrix4x4 m)
{
    return m.Transposed();
}