//
// Created by Killian on 16/04/2023.
//

#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;

    constexpr Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

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
};