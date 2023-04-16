//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "math/Vector2.h"

enum RendererType
{
    OpenGL,
    Vulkan,
    DirectX,
    Metal
};

class Renderer;

class Window
{
public:
    Window(const std::string_view& title, ::Vector2<uint32_t> size, bool fullscreen = false, bool vsync = true);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    std::shared_ptr<Renderer> CreateRenderer(RendererType type);

    void SetTitle(const std::string& title);
    void Resize(uint32_t width, uint32_t height);
    void SetFullscreen(bool fullscreen);
    void SetVSync(bool vsync);

    [[nodiscard]] ::Vector2<uint32_t> GetSize() const;
    [[nodiscard]] inline bool IsFullscreen() const { return m_fullscreen; }
    [[nodiscard]] inline std::string_view GetTitle() const { return m_title; };
    [[nodiscard]] inline bool IsVSync() const { return m_vsync; }

protected:
    friend class Renderer;

#ifdef PLATFORM_WIN32
    [[nodiscard]] inline HWND GetHandle() const { return m_windowHandle; }
#elif PLATFORM_LINUX
    [[nodiscard]] inline Display* GetDisplay() const { return m_display; }
    [[nodiscard]] inline Window GetHandle() const { return m_windowHandle; }
#elif PLATFORM_MACOS
#error "MacOS is not supported yet"
#endif

private:
#ifdef PLATFORM_WIN32
    HWND m_windowHandle;
#elif PLATFORM_LINUX
    Display* m_display;
    Window m_windowHandle;
#elif PLATFORM_MACOS
#error "MacOS is not supported yet"
#endif

    ::Vector2<uint32_t> m_size;
    std::string m_title;
    bool m_fullscreen;
    bool m_vsync;

    std::shared_ptr<Renderer> m_renderer;
};