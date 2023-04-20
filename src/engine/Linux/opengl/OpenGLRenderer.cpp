//
// Created by Killian on 16/04/2023.
//
#include "engine/glad/glad.h"
#include "engine/glad/glad_wgl.h"
#include "engine/renderers/opengl/OpenGLRenderer.h"
#include <stb_image.h>

#ifdef PLATFORM_LINUX

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

bool contextErrorOccured = false;
int ErrorHandler(Display* display, XErrorEvent* event)
{
    contextErrorOccured = true;

    std::string errorMessage(8192, '\0');
    XGetErrorText(display, event->error_code, errorMessage.data(), 8191);

    spdlog::info("XError : {}", errorMessage);

    return 0;
}

bool IsExtensionSupported(const char* extentionList, const char* extension)
{
    const char* where = std::strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    const char* terminator;
    for (const char* start = extentionList;;start = terminator)
    {
        where = std::strstr(start, extension);

        if(!where)
            break;

        terminator = where + std::strlen(extension);

        if((where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0'))
            return true;
    }

    return false;
}

Renderer::Renderer(Window &window)
    : Engine::Renderer(window)
{
    gladLoadGLX(GetDisplay(), DefaultScreen(GetDisplay()));

    int attributes[] = {
        GLX_X_RENDERABLE, True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        None
    };

    int glxMajor, glxMinor;

    if(!glXQueryVersion(GetDisplay(), &glxMajor, &glxMinor) || ((glxMajor == 1) && (glxMinor < 3)) || (glxMajor < 1))
    {
        throw std::runtime_error("Invalid GLX version");
    }

    int framebufferCount;
    GLXFBConfig* framebufferConfig = glXChooseFBConfig(GetDisplay(), DefaultScreen(GetDisplay()), attributes, &framebufferCount);
    if(!framebufferConfig)
    {
        throw std::runtime_error("Failed to retrieve a framebuffer config");
    }

    int bestFramebufferConfigIndex = -1, worstFramebufferConfigIndex = -1, bestSamplesIndex = -1, worstSamplesIndex = 999;

    for(int i = 0; i < framebufferCount; i++)
    {
        XVisualInfo* visualInfo = glXGetVisualFromFBConfig(GetDisplay(), framebufferConfig[i]);
        if(!visualInfo) continue;

        int sampleBuffer, samples;
        glXGetFBConfigAttrib(GetDisplay(), framebufferConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
        glXGetFBConfigAttrib(GetDisplay(), framebufferConfig[i], GLX_SAMPLES, &samples);

        if(bestFramebufferConfigIndex < 0 || sampleBuffer && samples > bestSamplesIndex)
            bestFramebufferConfigIndex = i, bestSamplesIndex = samples;
        if(worstFramebufferConfigIndex < 0 || !sampleBuffer || samples < worstSamplesIndex)
            worstFramebufferConfigIndex = i, worstSamplesIndex = samples;

        XFree(visualInfo);
    }

    GLXFBConfig bestFramebufferConfig = framebufferConfig[bestFramebufferConfigIndex];

    XFree(framebufferConfig);

    const char* glxExtensions = glXQueryExtensionsString(GetDisplay(), DefaultScreen(GetDisplay()));

    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ErrorHandler);

    if(!IsExtensionSupported(glxExtensions, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
    {
        throw std::runtime_error("glXCreateContextAttribsARB() not found");
    }

    int contextAttributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 5,
#ifdef DEBUG
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | GLX_CONTEXT_DEBUG_BIT_ARB,
#else
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
#endif
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    m_context = glXCreateContextAttribsARB(GetDisplay(), bestFramebufferConfig, nullptr, True, contextAttributes);

    XSync(GetDisplay(), False);
    XSetErrorHandler(oldHandler);

    if(contextErrorOccured || !m_context)
    {
        throw std::runtime_error("Failed to create OpenGL context");
    }

    if(!glXIsDirect(GetDisplay(), m_context))
    {
        throw std::runtime_error("Indirect GLX rendering context obtained");
    }

    glXMakeCurrent(GetDisplay(), GetWindowHandle(), m_context);

    int version = gladLoadGL();
    if(version == 0)
    {
        throw std::runtime_error("Failed to load OpenGL");
    }

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
    glXMakeCurrent(GetDisplay(), None, nullptr);
    glXDestroyContext(GetDisplay(), m_context);
}

void Renderer::SetVSync(bool vsync)
{
    if(glXSwapIntervalEXT)
        glXSwapIntervalEXT(GetDisplay(), GetWindowHandle(), vsync);
    else if(glXSwapIntervalMESA)
        glXSwapIntervalMESA(vsync);
    else if(glXSwapIntervalSGI)
        glXSwapIntervalSGI(vsync);
}

void Renderer::Clear(Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SwapBuffers()
{
    glXSwapBuffers(GetDisplay(), GetWindowHandle());
}

}

#endif