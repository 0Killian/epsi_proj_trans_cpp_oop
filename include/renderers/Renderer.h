//
// Created by Killian on 16/04/2023.
//

#pragma once

#include <Window.h>

class Renderer
{
public:
    explicit Renderer(::Window& window) : m_window(window) {}

    virtual void SetVSync(bool vsync) = 0;

protected:
    ::Window& m_window;
};