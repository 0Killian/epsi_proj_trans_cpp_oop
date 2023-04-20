//
// Created by Killian on 16/04/2023.
//
#include "engine/glad/glad.h"
#include "engine/glad/glad_wgl.h"
#include "engine/renderers/opengl/OpenGLRenderer.h"
#include <stb_image.h>

#ifdef PLATFORM_WINDOWS

namespace Engine::OpenGL
{

void DebugCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
{
    std::stringstream ss;
    ss << "[OpenGL Debug - ";

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        ss << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        ss << "Deprecated";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        ss << "Undefined Behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        ss << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        ss << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        ss << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        ss << "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        ss << "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        ss << "Other";
        break;
    default:
        ss << "Unknown";
        break;
    }

    ss << "] " << message;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        spdlog::critical(ss.str());
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        spdlog::error(ss.str());
        break;
    case GL_DEBUG_SEVERITY_LOW:
        spdlog::warn(ss.str());
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    default:
        spdlog::info(ss.str());
        break;
    }
}

Renderer::Renderer(Window &window)
    : Engine::Renderer(window)
{
    // The steps to create an OpenGL Context on Windows are a little bit tricky:
    // 1. Create a dummy OpenGL Context
    // 2. Get WGL Extensions and Pixel Format Extensions
    // 3. Create a real OpenGL Context
    //
    // The dummy OpenGL Context is used to get the WGL Extensions and Pixel Format Extensions
    // See https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Proper_Context_Creation

    // Get the device context
    m_deviceContext = GetDC(GetWindowHandle());

    // Determine the Pixel Format to use
    // This is the recommended format to use
    // See https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Pixel_Format
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cRedBits = 0,
        .cRedShift = 0,
        .cGreenBits = 0,
        .cGreenShift = 0,
        .cBlueBits = 0,
        .cBlueShift = 0,
        .cAlphaBits = 0,
        .cAlphaShift = 0,
        .cAccumBits = 0,
        .cAccumRedBits = 0,
        .cAccumGreenBits = 0,
        .cAccumBlueBits = 0,
        .cAccumAlphaBits = 0,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .cAuxBuffers = 0,
        .iLayerType = PFD_MAIN_PLANE,
        .bReserved = 0,
        .dwLayerMask = 0,
        .dwVisibleMask = 0,
        .dwDamageMask = 0
    };

    // Choose the best pixel format
    int pixelFormat = ChoosePixelFormat(m_deviceContext, &pfd);
    if (pixelFormat == 0)
    {
        throw std::runtime_error("Failed to choose a pixel format");
    }

    // Set the pixel format
    if (!SetPixelFormat(m_deviceContext, pixelFormat, &pfd))
    {
        throw std::runtime_error("Failed to set the pixel format");
    }

    // Create the rendering context
    HGLRC tmpRenderingContext = wglCreateContext(m_deviceContext);
    if (tmpRenderingContext == nullptr)
    {
        throw std::runtime_error("Failed to create the rendering context");
    }

    // Make the rendering context current
    wglMakeCurrent(m_deviceContext, tmpRenderingContext);

    // Load the Extensions
    if (!gladLoadWGL(m_deviceContext))
    {
        throw std::runtime_error("Failed to load WGL extensions");
    }

    int version = gladLoadGL();
    if (version == 0)
    {
        throw std::runtime_error("Failed to load OpenGL");
    }

    // Create the real OpenGL Context
    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
#ifdef DEBUG
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
#else
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#endif
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, nullptr, attributes);
    if (m_renderingContext == nullptr)
    {
        throw std::runtime_error("Failed to create the rendering context");
    }

    // Delete the dummy rendering context
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tmpRenderingContext);

    // Make the real rendering context current
    wglMakeCurrent(m_deviceContext, m_renderingContext);

    // Initialize STB Image
    stbi_set_flip_vertically_on_load(true);

    // Set OpenGL Starting State
    glDebugMessageCallback(DebugCallback, this);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

Renderer::~Renderer()
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(m_renderingContext);
    ReleaseDC(GetWindowHandle(), m_deviceContext);
}

void Renderer::SetVSync(bool vsync)
{
    wglSwapIntervalEXT(vsync ? 1 : 0);
}

void Renderer::Clear(Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffers()
{
    ::SwapBuffers(m_deviceContext);
}

}

#endif