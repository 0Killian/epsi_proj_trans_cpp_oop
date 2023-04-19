//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/math/Vector2.h"

namespace Engine
{

class Renderer;
enum class RendererAPI : uint8_t;

class Window
{
public:
    Window(const std::string_view& title, Vector2<uint32_t> size, bool fullscreen = false, bool vsync = true);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    std::shared_ptr<Renderer> CreateRenderer(RendererAPI type);

    void SetTitle(const std::string_view& title);
    void Resize(uint32_t width, uint32_t height);
    void SetFullscreen(bool fullscreen);
    void SetVSync(bool vsync);

    [[nodiscard]] Vector2<uint32_t> GetSize() const;
    [[nodiscard]] inline bool IsFullscreen() const { return m_fullscreen; }
    [[nodiscard]] inline std::string_view GetTitle() const { return m_title; };
    [[nodiscard]] inline bool IsVSync() const { return m_vsync; }

protected:
    friend class Renderer;

#if defined(PLATFORM_WINDOWS)
    [[nodiscard]] inline HWND GetHandle() const { return m_windowHandle; }
#elif defined(PLATFORM_LINUX)
    [[nodiscard]] inline Display* GetDisplay() const { return m_display; }
    [[nodiscard]] inline ::Window GetWindowHandle() const { return m_windowHandle; }
#elif defined(PLATFORM_MACOS)
#error "MacOS is not supported yet"
#endif

private:
#if defined(PLATFORM_WINDOWS)
    HWND m_windowHandle;
#elif defined(PLATFORM_LINUX)
    Display* m_display;
    ::Window m_windowHandle;
    Atom m_wmDeleteWindow;
    Atom m_wmProtocols;
#elif defined(PLATFORM_MACOS)
#error "MacOS is not supported yet"
#endif

    Vector2<uint32_t> m_size;
    std::string m_title;
    bool m_fullscreen;
    bool m_vsync;

    std::shared_ptr<Renderer> m_renderer;
};

}