//
// Created by Killian on 16/04/2023.
//
#include <renderers/vulkan/VulkanRenderer.h>
#include <renderers/opengl/OpenGLRenderer.h>
#include <renderers/directx/DirectXRenderer.h>
#include "Window.h"

#ifdef PLATFORM_WIN32

Window::Window(const std::string_view& title, Vector2<uint32_t> size, bool fullscreen, bool vsync)
    : m_vsync(vsync), m_size(size), m_fullscreen(fullscreen), m_title(title)
{
    const char CLASS_NAME[] = "WindowClass";

    // Create the window class
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_OWNDC,
        .lpfnWndProc = DefWindowProc,
        .cbClsExtra = 0,
        .cbWndExtra = sizeof(Window*),
        .hInstance = GetModuleHandle(nullptr),
        .hIcon = nullptr,
        .hCursor = nullptr,
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = CLASS_NAME,
        .hIconSm = nullptr
    };

    RegisterClassEx(&wc);


    // Create the window
    m_windowHandle = CreateWindowEx(
        0,
        CLASS_NAME,
        title.data(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<int>(size.x),
        static_cast<int>(size.y),
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );

    if (m_windowHandle == nullptr)
    {
        throw std::runtime_error("Failed to create window");
    }

    ShowWindow(m_windowHandle, SW_SHOW);
}

void Window::SetFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;

    if (m_fullscreen)
    {
        SetWindowLong(m_windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
    }
    else
    {
        SetWindowLong(m_windowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
        SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y), SWP_FRAMECHANGED);
    }
}

void Window::SetTitle(const std::string& title)
{
    m_title = title;
    SetWindowText(m_windowHandle, m_title.c_str());
}

void Window::Resize(uint32_t width, uint32_t height)
{
    m_size.x = width;
    m_size.y = height;

    if (!m_fullscreen)
    {
        SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y), SWP_FRAMECHANGED);
    }
}

Vector2<uint32_t> Window::GetSize() const
{
    if(m_fullscreen)
    {
        return {
            static_cast<uint32_t>(GetSystemMetrics(SM_CXSCREEN)),
            static_cast<uint32_t>(GetSystemMetrics(SM_CYSCREEN))
        };
    }

    return m_size;
}

#elif PLATFORM_LINUX

#elif PLATFORM_MACOS
#error "MacOS is not supported yet"
#endif

std::shared_ptr<Renderer> Window::CreateRenderer(RendererType type)
{
    switch (type)
    {
    case RendererType::OpenGL:
        m_renderer = std::make_shared<OpenGLRenderer>(*this);
        break;

    case RendererType::Vulkan:
        m_renderer = std::make_shared<VulkanRenderer>(*this);
        break;

    case RendererType::DirectX:
#ifdef PLATFORM_WIN32
        m_renderer = std::make_shared<DirectXRenderer>(*this);
        break;
#else
        throw std::runtime_error("DirectX is only supported on Windows");
#endif

    case RendererType::Metal:
#ifdef PLATFORM_MACOS
        m_renderer = std::make_shared<MetalRenderer>(m_windowHandle);
        break;
#else
        throw std::runtime_error("Metal is only supported on MacOS");
#endif

    default:
        throw std::runtime_error("Unknown renderer type");
    }

    m_renderer->SetVSync(m_vsync);

    return m_renderer;
}

void Window::SetVSync(bool vsync)
{
    m_vsync = vsync;
    if(m_renderer)
    {
        m_renderer->SetVSync(vsync);
    }
}