//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "Vector2.h"

namespace Engine
{

template <typename T>
class Vector3
{
public:
    T x;
    T y;
    T z;

    constexpr Vector3() : x(0), y(0), z(0) {};
    constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {};
    explicit constexpr Vector3(const Vector2<T>& other) : x(other.x), y(other.y), z(0) {};

    Vector3<T> operator-() const
    {
        return Vector3<T>(-x, -y, -z);
    }

    Vector3<T> operator+(const Vector3<T>& other) const
    {
        return Vector3<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector3<T> operator-(const Vector3<T>& other) const
    {
        return Vector3<T>(x - other.x, y - other.y, z - other.z);
    }

    Vector3<T> operator*(const Vector3<T>& other) const
    {
        return Vector3<T>(x * other.x, y * other.y, z * other.z);
    }

    Vector3<T> operator/(const Vector3<T>& other) const
    {
        return Vector3<T>(x / other.x, y / other.y, z / other.z);
    }

    Vector3<T> operator+(const T& other) const
    {
        return Vector3<T>(x + other, y + other, z + other);
    }

    Vector3<T> operator-(const T& other) const
    {
        return Vector3<T>(x - other, y - other, z - other);
    }

    Vector3<T> operator*(const T& other) const
    {
        return Vector3<T>(x * other, y * other, z * other);
    }

    Vector3<T> operator/(const T& other) const
    {
        return Vector3<T>(x / other, y / other, z / other);
    }

    Vector3<T>& operator+=(const Vector3<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3<T>& operator-=(const Vector3<T>& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3<T>& operator*=(const Vector3<T>& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3<T>& operator/=(const Vector3<T>& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3<T>& operator+=(const T& other)
    {
        x += other;
        y += other;
        z += other;
        return *this;
    }

    Vector3<T>& operator-=(const T& other)
    {
        x -= other;
        y -= other;
        z -= other;
        return *this;
    }

    Vector3<T>& operator*=(const T& other)
    {
        x *= other;
        y *= other;
        z *= other;
        return *this;
    }

    Vector3<T>& operator/=(const T& other)
    {
        x /= other;
        y /= other;
        z /= other;
        return *this;
    }

    bool operator==(const Vector3<T>& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3<T>& other) const
    {
        return !(*this == other);
    }

    T Magnitude2() const
    {
        return x * x + y * y + z * z;
    }

    T Magnitude() const
    {
        return std::sqrt(Magnitude2());
    }

    bool operator<(const Vector3<T>& other) const
    {
        return Magnitude2() < other.Magnitude2();
    }

    bool operator>(const Vector3<T>& other) const
    {
        return Magnitude2() > other.Magnitude2();
    }

    bool operator<=(const Vector3<T>& other) const
    {
        return Magnitude2() <= other.Magnitude2();
    }

    bool operator>=(const Vector3<T>& other) const
    {
        return Magnitude2() >= other.Magnitude2();
    }

    Vector3<T> Normalize() const
    {
        return *this / Magnitude();
    }

    Vector3<T> Absolute() const
    {
        return Vector3<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    Vector3<T> Floor() const
    {
        return Vector3<T>(std::floor(x), std::floor(y), std::floor(z));
    }

    Vector3<T> Ceil() const
    {
        return Vector3<T>(std::ceil(x), std::ceil(y), std::ceil(z));
    }

    Vector3<T> Round() const
    {
        return Vector3<T>(std::round(x), std::round(y), std::round(z));
    }

    Vector3<T> Min(const Vector3<T>& other) const
    {
        if(*this < other)
            return *this;
        return other;
    }

    Vector3<T> Max(const Vector3<T>& other) const
    {
        if(*this > other)
            return *this;
        return other;
    }

    Vector3<T> Clamp(const Vector3<T>& min, const Vector3<T>& max) const
    {
        return Min(max).Max(min);
    }

    T Dot(const Vector3<T>& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3<T> Cross(const Vector3<T>& other) const
    {
        return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    Vector3<T> Reflect(const Vector3<T>& normal) const
    {
        return *this - normal * Dot(normal) * 2;
    }

    // ???????
    Vector3<T> Refract(const Vector3<T>& normal, T eta) const
    {
        T dot = Dot(normal);
        T k = 1 - eta * eta * (1 - dot * dot);
        if(k < 0)
            return Vector3<T>(0, 0, 0);
        return *this * eta - normal * (eta * dot + std::sqrt(k));
    }

    Vector3<T> Lerp(const Vector3<T>& other, T t) const
    {
        return *this + (other - *this) * t;
    }

    Vector3<T> Slerp(const Vector3<T>& other, T t) const
    {
        T dot = Dot(other);
        std::clamp(dot, -1, 1);
        T theta = std::acos(dot) * t;
        Vector3<T> relativeVec = (other - *this * dot).Normalized();
        return (*this * std::cos(theta) + relativeVec * std::sin(theta));
    }

};

}