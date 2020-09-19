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
    float xScale, yScale, C,  D;
    yScale = 1 / tan(FOV / 2);
    xScale = yScale / screenAspect;


    C = screenDepth / (screenDepth - screenNear);

    D = -screenNear * screenDepth / (screenDepth - screenNear);


    return Matrix4x4(
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, C, 1,
        0, 0, D, 0);
}

Matrix4x4 EngineMath::OrthographicProjectionMatrix(float screenWidth, float screenHeight, float screenNear, float screenDepth)
{
    float A, B, C, D, E;
    A = 2 / screenWidth;
    B = 2 / screenHeight;
    C = 1 / (screenDepth - screenNear);
    D = 1;
    E = -screenNear / (screenDepth - screenNear);


    return (Matrix4x4(
        Vector4(A, 0, 0, 0),
        Vector4(0, B, 0, 0),
        Vector4(0, 0, C, 0),
        Vector4(0, 0, E, D))) ;//* Matrix4x4(-1,0,0,0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ))* Matrix4x4(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);


}

Matrix4x4 EngineMath::Identity()
{
    return Matrix4x4(
        Vector4(1, 0, 0, 0),
        Vector4(0, 1, 0, 0),
        Vector4(0, 0, 1, 0),
        Vector4(0, 0, 0, 1));
}

Matrix4x4 EngineMath::LookAtMatrix(Vector3 Eye, Vector3 At, Vector3 up)
{
    Vector3 zaxis, xaxis, yaxis;
    zaxis = Normalized(At - Eye );
    xaxis = Normalized(zaxis.CrossProdut(up,  zaxis));
    yaxis = zaxis.CrossProdut(zaxis, xaxis);
    Matrix4x4 a = 
     Matrix4x4(
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
        -(xaxis * Eye), -(yaxis * Eye), -(zaxis * Eye), 1);
    return a;
}

Matrix4x4 EngineMath::RotationPitchYawRoll(float pitch, float yaw, float roll) // p = x Y = y r = z
{

    Matrix4x4 p, y, r;
    y = Matrix4x4(
        cos(pitch), 0, sin(pitch), 0,
        0, 1, 0, 0,
        - sin(pitch), 0, cos(pitch), 0,
        0, 0, 0, 1);
    r = Matrix4x4(
        cos(yaw), -sin(yaw), 0, 0,
        sin(yaw), cos(yaw), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    p = Matrix4x4(
        1, 0, 0, 0,
        0, cos(roll), -sin(roll),0,
        0, sin(roll), cos(roll), 0,
        0, 0, 0, 1);


    return (r* p)*y;
}
Matrix4x4 EngineMath::GetTramsformationMatrix(Vector3 posistion, Vector3 Rotation, Vector3 Scale)
{
    Matrix4x4 scale, translation, rotation;
    scale = Matrix4x4(
        Scale.x, 0, 0, 0,
        0, Scale.y, 0, 0,
        0, 0, Scale.z, 0,
        0, 0, 0, 1);
    translation = Matrix4x4(
        1, 0, 0, posistion.x,
        0, 1, 0, posistion.y,
        0, 0, 1, posistion.z,
        0, 0, 0, 1);
    float yaw, pitch, roll;
    pitch = Rotation.x * 0.0174532925f;
    yaw = Rotation.y * 0.0174532925f;
    roll = Rotation.z * 0.0174532925f;
    rotation = RotationPitchYawRoll(pitch, yaw, roll);
    auto a =   rotation * translation;
    return (translation*scale)*rotation;
}

float EngineMath::PlaneDotCoord(Plane plane, Vector3 point)
{
    return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d * 1;
}
Vector3 EngineMath::Normalized(Vector3 v)
{
    if(v != Vector3(0,0,0))
    return v / v.Length();
    return Vector3(0, 0, 0);
}

Matrix4x4 EngineMath::Transpose(Matrix4x4 m)
{
    return m.Transposed();
}