//
// Created by Killian on 16/04/2023.
//

#pragma once

namespace Engine
{

template <typename T>
requires std::is_arithmetic_v<T>
class Vector2
{
public:
    T x;
    T y;

    constexpr Vector2() : x(0), y(0) {};
    constexpr Vector2(T x, T y) : x(x), y(y) {};

    template <typename U>
    requires std::is_arithmetic_v<U>
    constexpr explicit Vector2(const Vector2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {};

    template <typename U>
    requires std::is_arithmetic_v<U>
    constexpr Vector2& operator=(const Vector2<U>& other)
    {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        return *this;
    }

    constexpr Vector2<T> operator-() const
    {
        return Vector2<T>(-x, -y);
    }

    constexpr Vector2<T> operator+(const Vector2<T>& other) const
    {
        return Vector2<T>(x + other.x, y + other.y);
    }

    constexpr Vector2<T> operator-(const Vector2<T>& other) const
    {
        return Vector2<T>(x - other.x, y - other.y);
    }

    constexpr Vector2<T> operator*(const Vector2<T>& other) const
    {
        return Vector2<T>(x * other.x, y * other.y);
    }

    constexpr Vector2<T> operator/(const Vector2<T>& other) const
    {
        return Vector2<T>(x / other.x, y / other.y);
    }

    constexpr Vector2<T> operator+(const T& other) const
    {
        return Vector2<T>(x + other, y + other);
    }

    constexpr Vector2<T> operator-(const T& other) const
    {
        return Vector2<T>(x - other, y - other);
    }

    constexpr Vector2<T> operator*(const T& other) const
    {
        return Vector2<T>(x * other, y * other);
    }

    constexpr Vector2<T> operator/(const T& other) const
    {
        return Vector2<T>(x / other, y / other);
    }

    constexpr Vector2<T>& operator+=(const Vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector2<T>& operator-=(const Vector2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector2<T>& operator*=(const Vector2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    constexpr Vector2<T>& operator/=(const Vector2<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    constexpr Vector2<T>& operator+=(const T& other)
    {
        x += other;
        y += other;
        return *this;
    }

    constexpr Vector2<T>& operator-=(const T& other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    constexpr Vector2<T>& operator*=(const T& other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    constexpr Vector2<T>& operator/=(const T& other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    constexpr bool operator==(const Vector2<T>& other) const
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vector2<T>& other) const
    {
        return !(*this == other);
    }

    constexpr T Magnitude2() const
    {
        return x * x + y * y;
    }

    constexpr T Magnitude() const
    {
        return std::sqrt(Magnitude2());
    }

    constexpr bool operator<(const Vector2<T>& other) const
    {
        return Magnitude2() < other.Magnitude2();
    }

    constexpr bool operator>(const Vector2<T>& other) const
    {
        return Magnitude2() > other.Magnitude2();
    }

    constexpr bool operator<=(const Vector2<T>& other) const
    {
        return Magnitude2() <= other.Magnitude2();
    }

    constexpr bool operator>=(const Vector2<T>& other) const
    {
        return Magnitude2() >= other.Magnitude2();
    }

    constexpr Vector2<T> Normalize() const
    {
        return *this / Magnitude();
    }

    constexpr Vector2<T> Absolute() const
    {
        return Vector2<T>(std::abs(x), std::abs(y));
    }

    constexpr Vector2<T> Floor() const
    {
        return Vector2<T>(std::floor(x), std::floor(y));
    }

    constexpr Vector2<T> Ceil() const
    {
        return Vector3<T>(std::ceil(x), std::ceil(y));
    }

    constexpr Vector2<T> Round() const
    {
        return Vector2<T>(std::round(x), std::round(y));
    }

    constexpr Vector2<T> Min(const Vector2<T>& other) const
    {
        if(*this < other)
            return *this;
        return other;
    }

    constexpr Vector2<T> Max(const Vector2<T>& other) const
    {
        if(*this > other)
            return *this;
        return other;
    }

    constexpr Vector2<T> Clamp(const Vector2<T>& min, const Vector2<T>& max) const
    {
        return Min(max).Max(min);
    }

    constexpr T Dot(const Vector2<T>& other) const
    {
        return x * other.x + y * other.y;
    }

    constexpr Vector2<T> Reflect(const Vector2<T>& normal) const
    {
        return *this - normal * 2 * Dot(normal);
    }

    // ???????
    constexpr Vector2<T> Refract(const Vector2<T>& normal, T eta) const
    {
        T dot = Dot(normal);
        T k = 1 - eta * eta * (1 - dot * dot);
        if(k < 0)
            return Vector2<T>(0, 0);
        else
            return *this * eta - normal * (eta * dot + std::sqrt(k));
    }

    constexpr Vector2<T> Lerp(const Vector2<T>& other, T t) const
    {
        return *this + (other - *this) * t;
    }

    constexpr Vector2<T> Slerp(const Vector2<T>& other, T t) const
    {
        T dot = Dot(other);
        std::clamp(dot, -1, 1);
        T theta = std::acos(dot) * t;
        Vector2<T> relativeVec = (other - *this * dot).Normalized();
        return (*this * std::cos(theta) + relativeVec * std::sin(theta));
    }

};

}