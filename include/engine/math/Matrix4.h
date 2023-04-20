//
// Created by Killian on 19/04/2023.
//

#pragma once

#include "Vector4.h"

namespace Engine
{

template <typename T>
class Matrix4
{
public:
    T data[4][4];

    constexpr Matrix4() : data {0} {};
    explicit constexpr Matrix4(T data[4][4]) : data {0}
    {
        this->data[0][0] = data[0][0]; this->data[0][1] = data[0][1]; this->data[0][2] = data[0][2]; this->data[0][3] = data[0][3];
        this->data[1][0] = data[1][0]; this->data[1][1] = data[1][1]; this->data[1][2] = data[1][2]; this->data[1][3] = data[1][3];
        this->data[2][0] = data[2][0]; this->data[2][1] = data[2][1]; this->data[2][2] = data[2][2]; this->data[2][3] = data[2][3];
        this->data[3][0] = data[3][0]; this->data[3][1] = data[3][1]; this->data[3][2] = data[3][2]; this->data[3][3] = data[3][3];
    };

    Matrix4<T> operator*(const Matrix4<T>& other)
    {
        Matrix4<T> result;

        result.data[0][0] = data[0][0] * other.data[0][0] + data[1][0] * other.data[0][1] + data[2][0] * other.data[0][2] + data[3][0] * other.data[0][3];
        result.data[0][1] = data[0][1] * other.data[0][0] + data[1][1] * other.data[0][1] + data[2][1] * other.data[0][2] + data[3][1] * other.data[0][3];
        result.data[0][2] = data[0][2] * other.data[0][0] + data[1][2] * other.data[0][1] + data[2][2] * other.data[0][2] + data[3][2] * other.data[0][3];
        result.data[0][3] = data[0][3] * other.data[0][0] + data[1][3] * other.data[0][1] + data[2][3] * other.data[0][2] + data[3][3] * other.data[0][3];

        result.data[1][0] = data[0][0] * other.data[1][0] + data[1][0] * other.data[1][1] + data[2][0] * other.data[1][2] + data[3][0] * other.data[1][3];
        result.data[1][1] = data[0][1] * other.data[1][0] + data[1][1] * other.data[1][1] + data[2][1] * other.data[1][2] + data[3][1] * other.data[1][3];
        result.data[1][2] = data[0][2] * other.data[1][0] + data[1][2] * other.data[1][1] + data[2][2] * other.data[1][2] + data[3][2] * other.data[1][3];
        result.data[1][3] = data[0][3] * other.data[1][0] + data[1][3] * other.data[1][1] + data[2][3] * other.data[1][2] + data[3][3] * other.data[1][3];

        result.data[2][0] = data[0][0] * other.data[2][0] + data[1][0] * other.data[2][1] + data[2][0] * other.data[2][2] + data[3][0] * other.data[2][3];
        result.data[2][1] = data[0][1] * other.data[2][0] + data[1][1] * other.data[2][1] + data[2][1] * other.data[2][2] + data[3][1] * other.data[2][3];
        result.data[2][2] = data[0][2] * other.data[2][0] + data[1][2] * other.data[2][1] + data[2][2] * other.data[2][2] + data[3][2] * other.data[2][3];
        result.data[2][3] = data[0][3] * other.data[2][0] + data[1][3] * other.data[2][1] + data[2][3] * other.data[2][2] + data[3][3] * other.data[2][3];

        result.data[3][0] = data[0][0] * other.data[3][0] + data[1][0] * other.data[3][1] + data[2][0] * other.data[3][2] + data[3][0] * other.data[3][3];
        result.data[3][1] = data[0][1] * other.data[3][0] + data[1][1] * other.data[3][1] + data[2][1] * other.data[3][2] + data[3][1] * other.data[3][3];
        result.data[3][2] = data[0][2] * other.data[3][0] + data[1][2] * other.data[3][1] + data[2][2] * other.data[3][2] + data[3][2] * other.data[3][3];
        result.data[3][3] = data[0][3] * other.data[3][0] + data[1][3] * other.data[3][1] + data[2][3] * other.data[3][2] + data[3][3] * other.data[3][3];

        return result;
    }

    Vector4<T> operator*(const Vector4<T>& other)
    {
        Vector4<T> result;

        result.x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z + data[0][3] * other.w;
        result.y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z + data[1][3] * other.w;
        result.z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z + data[2][3] * other.w;
        result.w = data[3][0] * other.x + data[3][1] * other.y + data[3][2] * other.z + data[3][3] * other.w;

        return result;
    }

    static Matrix4<T> Identity()
    {
        Matrix4<T> result;

        result.data[0][0] = 1;
        result.data[1][1] = 1;
        result.data[2][2] = 1;
        result.data[3][3] = 1;

        return result;
    }

    static Matrix4<T> Translation(const Vector3<T>& translation)
    {
        Matrix4<T> result = Matrix4<T>::Identity();

        result.data[3][0] = translation.x;
        result.data[3][1] = translation.y;
        result.data[3][2] = translation.z;

        return result;
    }

    static Matrix4<T> Translation(T x, T y, T z) { return Translation(Vector3<T>(x, y, z)); }

    static Matrix4<T> Rotation(const Vector3<T>& rotation)
    {
        Matrix4<T> rx = Matrix4<T>::Identity();
        Matrix4<T> ry = Matrix4<T>::Identity();
        Matrix4<T> rz = Matrix4<T>::Identity();

        T x = rotation.x * static_cast<T>(M_PI) / 180.0f;
        T y = rotation.y * static_cast<T>(M_PI) / 180.0f;
        T z = rotation.z * static_cast<T>(M_PI) / 180.0f;

        rx.data[1][1] = std::cos(x);
        rx.data[1][2] = -std::sin(x);
        rx.data[2][1] = std::sin(x);
        rx.data[2][2] = std::cos(x);

        ry.data[0][0] = std::cos(y);
        ry.data[0][2] = -std::sin(y);
        ry.data[2][0] = std::sin(y);
        ry.data[2][2] = std::cos(y);

        rz.data[0][0] = std::cos(z);
        rz.data[0][1] = -std::sin(z);
        rz.data[1][0] = std::sin(z);
        rz.data[1][1] = std::cos(z);

        return rz * ry * rx;
    }

    static Matrix4<T> Rotation(T x, T y, T z) { return Rotation(Vector3<T>(x, y, z)); }

    static Matrix4<T> Rotation(const Vector3<T>& axis, T angle)
    {
        Matrix4<T> result = Matrix4<T>::Identity();

        T c = cos(angle);
        T s = sin(angle);
        T t = 1 - c;

        result.data[0][0] = t * axis.x * axis.x + c;
        result.data[0][1] = t * axis.x * axis.y + s * axis.z;
        result.data[0][2] = t * axis.x * axis.z - s * axis.y;

        result.data[1][0] = t * axis.x * axis.y - s * axis.z;
        result.data[1][1] = t * axis.y * axis.y + c;
        result.data[1][2] = t * axis.y * axis.z + s * axis.x;

        result.data[2][0] = t * axis.x * axis.z + s * axis.y;
        result.data[2][1] = t * axis.y * axis.z - s * axis.x;
        result.data[2][2] = t * axis.z * axis.z + c;

        return result;
    }

    static Matrix4<T> Scale(const Vector3<T>& scale)
    {
        Matrix4<T> result = Matrix4<T>::Identity();

        result.data[0][0] = scale.x;
        result.data[1][1] = scale.y;
        result.data[2][2] = scale.z;

        return result;
    }

    static Matrix4<T> PerspectiveProjection(T fov, T aspectRatio, T near, T far)
    {
        Matrix4<T> result = Matrix4<T>::Identity();

        T tanHalfFov = std::tan(fov * M_PI / 360.0f);
        T zRange = near - far;

        result.data[0][0] = 1 / (tanHalfFov * aspectRatio);
        result.data[1][1] = 1 / tanHalfFov;
        result.data[2][2] = (-near - far) / zRange;
        result.data[3][2] = 2 * far * near / zRange;
        result.data[2][3] = 1;
        result.data[3][3] = 0;

        return result;

        /*T f = 1.0f / std::tan(fov * M_PI / 360.0f);

        result.data[0][0] = f / aspectRatio;
        result.data[1][1] = f;
        result.data[2][2] = (far + near) / (near - far);
        result.data[2][3] = -1;
        result.data[3][2] = (2 * far * near) / (near - far);
        result.data[3][3] = 0;

        return result;*/
    }

    static Matrix4<T> OrthographicProjection(T left, T right, T bottom, T top, T near, T far)
    {
        Matrix4<T> result = Matrix4<T>::Identity();

        result.data[0][0] = 2 / (right - left);
        result.data[1][1] = 2 / (top - bottom);
        result.data[2][2] = -2 / (far - near);
        result.data[0][3] = -(right + left) / (right - left);
        result.data[1][3] = -(top + bottom) / (top - bottom);
        result.data[2][3] = -(far + near) / (far - near);

        return result;
    }

    static Matrix4<T> LookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up)
    {
        Vector3<T> f = (center - eye).Normalize();
        Vector3<T> s = up.Cross(f).Normalize();
        Vector3<T> u = f.Cross(s);

        Matrix4<T> result = Matrix4<T>::Identity();

        result.data[0][0] = s.x;
        result.data[1][0] = s.y;
        result.data[2][0] = s.z;
        result.data[3][0] = -s.Dot(eye);

        result.data[0][1] = u.x;
        result.data[1][1] = u.y;
        result.data[2][1] = u.z;
        result.data[3][1] = -u.Dot(eye);

        result.data[0][2] = f.x;
        result.data[1][2] = f.y;
        result.data[2][2] = f.z;
        result.data[3][2] = -f.Dot(eye);

        return result;
    }
};

}
