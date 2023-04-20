//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/math/Vector4.h"

namespace Engine
{

struct Color
{
    float r;
    float g;
    float b;
    float a;

    constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    constexpr Color() : Color(0.0f, 0.0f, 0.0f, 1.0f) {}

    static constexpr Color Black() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color White() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr Color Red() { return {1.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color Green() { return {0.0f, 1.0f, 0.0f, 1.0f}; }
    static constexpr Color Blue() { return {0.0f, 0.0f, 1.0f, 1.0f}; }
    static constexpr Color Yellow() { return {1.0f, 1.0f, 0.0f, 1.0f}; }
    static constexpr Color Cyan() { return {0.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr Color Magenta() { return {1.0f, 0.0f, 1.0f, 1.0f}; }
    static constexpr Color Orange() { return {1.0f, 0.5f, 0.0f, 1.0f}; }
    static constexpr Color Purple() { return {0.5f, 0.0f, 1.0f, 1.0f}; }
    static constexpr Color Pink() { return {1.0f, 0.0f, 0.5f, 1.0f}; }
    static constexpr Color Brown() { return {0.5f, 0.25f, 0.0f, 1.0f}; }
    static constexpr Color Gray() { return {0.5f, 0.5f, 0.5f, 1.0f}; }
    static constexpr Color LightGray() { return {0.75f, 0.75f, 0.75f, 1.0f}; }
    static constexpr Color DarkGray() { return {0.25f, 0.25f, 0.25f, 1.0f}; }
    static constexpr Color Transparent() { return {0.0f, 0.0f, 0.0f, 0.0f}; }

    [[nodiscard]] constexpr Vector4<float> ToVector4() const { return {r, g, b, a}; }
};

}