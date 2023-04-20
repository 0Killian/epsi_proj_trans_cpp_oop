//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "Vector3.h"

namespace Engine
{

// The fourth component is used to determine if the vector is a point in space or a direction.
template <typename T>
class Vector4 : public Vector3<T>
{
public:
    T w;

    constexpr Vector4() : Vector3<T>(), w(0) {};
    constexpr Vector4(T x, T y, T z, T w) : Vector3<T>(x, y, z), w(w) {};
    explicit constexpr Vector4(const Vector2<T>& other) : Vector3<T>(other), w(0) {};
    explicit constexpr Vector4(const Vector3<T>& other) : Vector3<T>(other), w(0) {};
};

}