//
// Created by Killian on 18/04/2023.
//
#include "engine/Window.h"
#include "engine/renderers/Renderer.h"
#include "engine/renderers/opengl/OpenGLRenderer.h"
#include "engine/renderers/vulkan/VulkanRenderer.h"

#ifdef PLATFORM_LINUX
namespace Engine
{

Window::Window(const std::string_view& title, Vector2<uint32_t> size, bool fullscreen, bool vsync)
        : m_title(title), m_size(size), m_fullscreen(fullscreen), m_vsync(vsync)
{
    m_display = XOpenDisplay(nullptr);
    if (m_display == nullptr)
    {
        throw std::runtime_error("Failed to open X11 display");
    }

    // TODO: This call is global to the system, check if we can make it per window
    XAutoRepeatOff(m_display);

    XSetWindowAttributes windowAttributes = {};
    windowAttributes.event_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | ExposureMask |
                                  PointerMotionMask | StructureNotifyMask;
    windowAttributes.background_pixel = BlackPixel(m_display, DefaultScreen(m_display));
    windowAttributes.background_pixmap = None;
    windowAttributes.border_pixel = 0;
    windowAttributes.colormap = XCreateColormap(m_display, RootWindow(m_display, DefaultScreen(m_display)),
                                                DefaultVisual(m_display, DefaultScreen(m_display)), AllocNone);

    m_windowHandle = XCreateWindow(
            m_display,
            RootWindow(m_display, DefaultScreen(m_display)),
            0,
            0,
            size.x,
            size.y,
            0,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            DefaultVisual(m_display, DefaultScreen(m_display)),
            CWBackPixel | CWEventMask,
            &windowAttributes);

    if(!m_windowHandle)
    {
        throw std::runtime_error("Failed to create X11 window");
    }

    SetTitle(m_title);
    SetFullscreen(m_fullscreen);

    m_wmDeleteWindow = XInternAtom(m_display, "WM_DELETE_WINDOW", 0);
    m_wmProtocols = XInternAtom(m_display, "WM_PROTOCOLS", 0);

    XSetWMProtocols(m_display, m_windowHandle, &m_wmDeleteWindow, 1);
    XMapWindow(m_display, m_windowHandle);
}

void Window::SetTitle(const std::string_view& title)
{
    m_title = title;
    XStoreName(m_display, m_windowHandle, m_title.c_str());
}

Window::~Window()
{
    XDestroyWindow(m_display, m_windowHandle);
    XCloseDisplay(m_display);
}

void Window::Resize(uint32_t width, uint32_t height)
{
    m_size.x = width;
    m_size.y = height;

    XResizeWindow(m_display, m_windowHandle, m_size.x, m_size.y);
}

void Window::SetFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;

    if (m_fullscreen)
    {
        XMoveResizeWindow(
                m_display,
                m_windowHandle,
                0, 0,
                DefaultScreenOfDisplay(m_display)->width, DefaultScreenOfDisplay(m_display)->height);
    }
    else
    {
        XMoveResizeWindow(m_display, m_windowHandle, 0, 0, m_size.x, m_size.y);
    }
}

void Window::SetVSync(bool vsync)
{
    m_vsync = vsync;
    if(m_renderer)
        m_renderer->SetVSync(vsync);
}

[[nodiscard]] Vector2<uint32_t> Window::GetSize() const
{
    if(m_fullscreen)
        return {DefaultScreenOfDisplay(m_display)->width, DefaultScreenOfDisplay(m_display)->height};

    return m_size;
}

std::shared_ptr<Renderer> Window::CreateRenderer(RendererAPI type)
{
    switch(type)
    {
    case RendererAPI::RendererAPI_OpenGL:
        m_renderer = std::make_shared<OpenGL::Renderer>(*this);
        break;
    case RendererAPI::RendererAPI_Vulkan:
        m_renderer = std::make_shared<Vulkan::Renderer>(*this);
        break;
    case RendererAPI::RendererAPI_DirectX:
        throw std::runtime_error("DirectX is not supported on Linux");
    case RendererAPI::RendererAPI_Metal:
        throw std::runtime_error("Metal is not supported on Linux");
    default:
        return nullptr;
    }

    m_renderer->SetVSync(m_vsync);

    return m_renderer;
}

}
#endif