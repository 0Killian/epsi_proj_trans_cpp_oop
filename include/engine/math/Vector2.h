//
// Created by Killian on 16/04/2023.
//

#pragma once

namespace Engine
{

template <typename T>
class Vector2
{
public:
    T x;
    T y;

    Vector2() : x(0), y(0) {};
    Vector2(T x, T y) : x(x), y(y) {};

    Vector2<T> operator+(const Vector2<T>& other) const
    {
        return Vector2<T>(x + other.x, y + other.y);
    }

    Vector2<T> operator-(const Vector2<T>& other) const
    {
        return Vector2<T>(x - other.x, y - other.y);
    }

    Vector2<T> operator*(const Vector2<T>& other) const
    {
        return Vector2<T>(x * other.x, y * other.y);
    }

    Vector2<T> operator/(const Vector2<T>& other) const
    {
        return Vector2<T>(x / other.x, y / other.y);
    }

    Vector2<T> operator+(const T& other) const
    {
        return Vector2<T>(x + other, y + other);
    }

    Vector2<T> operator-(const T& other) const
    {
        return Vector2<T>(x - other, y - other);
    }

    Vector2<T> operator*(const T& other) const
    {
        return Vector2<T>(x * other, y * other);
    }

    Vector2<T> operator/(const T& other) const
    {
        return Vector2<T>(x / other, y / other);
    }

    Vector2<T>& operator+=(const Vector2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2<T>& operator-=(const Vector2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2<T>& operator*=(const Vector2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vector2<T>& operator/=(const Vector2<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vector2<T>& operator+=(const T& other)
    {
        x += other;
        y += other;
        return *this;
    }

    Vector2<T>& operator-=(const T& other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    Vector2<T>& operator*=(const T& other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    Vector2<T>& operator/=(const T& other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    bool operator==(const Vector2<T>& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2<T>& other) const
    {
        return *this != other;
    }

    T mag2() const
    {
        return x * x + y * y;
    }

    T mag() const
    {
        return sqrt(mag2());
    }

    bool operator<(const Vector2<T>& other) const
    {
        return mag2() < other.mag2();
    }

    bool operator>(const Vector2<T>& other) const
    {
        return mag2() > other.mag2();
    }

    bool operator<=(const Vector2<T>& other) const
    {
        return mag2() <= other.mag2();
    }

    bool operator>=(const Vector2<T>& other) const
    {
        return mag2() >= other.mag2();
    }

    Vector2<T> norm() const
    {
        return *this / mag();
    }

    Vector2<T> abs() const
    {
        return Vector2<T>(std::abs(x), std::abs(y));
    }

    Vector2<T> floor() const
    {
        return Vector2<T>(std::floor(x), std::floor(y));
    }

    Vector2<T> ceil() const
    {
        return Vector2<T>(std::ceil(x), std::ceil(y));
    }

    Vector2<T> round() const
    {
        return Vector2<T>(std::round(x), std::round(y));
    }

    Vector2<T> min(const Vector2<T>& other) const
    {
        if(*this < other)
            return *this;
        return other;
    }

    Vector2<T> max(const Vector2<T>& other) const
    {
        if(*this > other)
            return *this;
        return other;
    }

};

}