//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "Vector3.h"

namespace Engine
{

// The fourth component is used to determine if the vector is a point in space or a direction.
template <typename T>
requires std::is_arithmetic_v<T>
class Vector4 : public Vector3<T>
{
public:
    T w;

    constexpr Vector4() : Vector3<T>(), w(0) {};
    constexpr Vector4(T x, T y, T z, T w) : Vector3<T>(x, y, z), w(w) {};
    constexpr explicit Vector4(const Vector2<T>& other) : Vector3<T>(other), w(0) {};
    constexpr explicit Vector4(const Vector3<T>& other) : Vector3<T>(other), w(0) {};

    template <typename U>
    requires std::is_arithmetic_v<U>
    constexpr explicit Vector4(const Vector4<U>& other) : Vector3<T>(other), w(static_cast<T>(other.w)) {};

    template <typename U>
    requires std::is_arithmetic_v<U>
    constexpr explicit Vector4(const Vector3<U>& other) : Vector3<T>(other), w(0) {};

    template <typename U>
    requires std::is_arithmetic_v<U>
    constexpr explicit Vector4(const Vector2<U>& other) : Vector3<T>(other), w(0) {};
};

}