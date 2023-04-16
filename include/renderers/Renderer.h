//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "Window.h"
#include "renderers/Color.h"

class Renderer
{
public:
    explicit Renderer(::Window& window) : m_window(window) {}

    virtual ~Renderer() = default;

    virtual void SetVSync(bool vsync) = 0;
    virtual void Clear(::Color color) = 0;
    virtual void SwapBuffers() = 0;

protected:
#ifdef PLATFORM_WIN32
    [[nodiscard]] inline HWND GetWindowHandle() const { return m_window.GetHandle(); }
#elif PLATFORM_LINUX
    [[nodiscard]] inline Display* GetDisplay() const { return m_window.GetDisplay(); }
    [[nodiscard]] inline Window GetWindowHandle() const { return m_window.GetHandle(); }
#elif PLATFORM_MACOS
#error "MacOS is not supported yet"
#endif

    ::Window& m_window;
};