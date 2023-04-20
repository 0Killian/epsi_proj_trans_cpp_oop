//
// Created by Killian on 19/04/2023.
//

#pragma once

#include "Vector3.h"

namespace Engine
{

template <typename T>
class Matrix3
{
public:
    T data[3][3];

    constexpr Matrix3() : data{0} {}
    constexpr explicit Matrix3(T data[3][3])
    {
        this->data[0][0] = data[0][0]; this->data[0][1] = data[0][1]; this->data[0][2] = data[0][2];
        this->data[1][0] = data[1][0]; this->data[1][1] = data[1][1]; this->data[1][2] = data[1][2];
        this->data[2][0] = data[2][0]; this->data[2][1] = data[2][1]; this->data[2][2] = data[2][2];
    }

    constexpr Matrix3<T> operator*(const Matrix3<T>& other) const
    {
        Matrix3<T> result;

        result.data[0][0] = data[0][0] * other.data[0][0] + data[0][1] * other.data[1][0] + data[0][2] * other.data[2][0];
        result.data[0][1] = data[0][0] * other.data[0][1] + data[0][1] * other.data[1][1] + data[0][2] * other.data[2][1];
        result.data[0][2] = data[0][0] * other.data[0][2] + data[0][1] * other.data[1][2] + data[0][2] * other.data[2][2];

        result.data[1][0] = data[1][0] * other.data[0][0] + data[1][1] * other.data[1][0] + data[1][2] * other.data[2][0];
        result.data[1][1] = data[1][0] * other.data[0][1] + data[1][1] * other.data[1][1] + data[1][2] * other.data[2][1];
        result.data[1][2] = data[1][0] * other.data[0][2] + data[1][1] * other.data[1][2] + data[1][2] * other.data[2][2];

        result.data[2][0] = data[2][0] * other.data[0][0] + data[2][1] * other.data[1][0] + data[2][2] * other.data[2][0];
        result.data[2][1] = data[2][0] * other.data[0][1] + data[2][1] * other.data[1][1] + data[2][2] * other.data[2][1];
        result.data[2][2] = data[2][0] * other.data[0][2] + data[2][1] * other.data[1][2] + data[2][2] * other.data[2][2];

        return result;
    }

    constexpr Vector3<T> operator*(const Vector3<T>& other) const
    {
        Vector3<T> result;

        result.x = data[0][0] * other.x + data[0][1] * other.y + data[0][2] * other.z;
        result.y = data[1][0] * other.x + data[1][1] * other.y + data[1][2] * other.z;
        result.z = data[2][0] * other.x + data[2][1] * other.y + data[2][2] * other.z;

        return result;
    }

    constexpr static Matrix3<T> Identity()
    {
        Matrix3<T> result;

        result.data[0][0] = 1;
        result.data[1][1] = 1;
        result.data[2][2] = 1;

        return result;
    }

};

}