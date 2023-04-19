//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/Window.h"
#include "Color.h"

namespace Engine
{

enum class RendererAPI : uint8_t
{
    RendererAPI_DirectX,
    RendererAPI_OpenGL,
    RendererAPI_Vulkan,
    RendererAPI_Metal
};

class Renderer
{
public:
    explicit Renderer(Window &window) : m_window(window) {}

    virtual ~Renderer() = default;

    virtual void SetVSync(bool vsync) = 0;
    virtual void Clear(Color color) = 0;
    virtual void SwapBuffers() = 0;

    [[nodiscard]] virtual RendererAPI GetAPI() const = 0;

protected:
#if defined(PLATFORM_WINDOWS)
    [[nodiscard]] inline HWND GetWindowHandle() const { return m_window.GetHandle(); }
#elif defined(PLATFORM_LINUX)
    [[nodiscard]] inline Display* GetDisplay() const { return m_window.GetDisplay(); }
    [[nodiscard]] inline ::Window GetWindowHandle() const { return m_window.GetWindowHandle(); }
#elif defined(PLATFORM_MACOS)
#error "MacOS is not supported yet"
#endif

    Window &m_window;
};

}