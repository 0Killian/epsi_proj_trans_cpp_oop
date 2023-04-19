//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/renderers/Renderer.h"
#include "engine/glad/glad_glx.h"

namespace Engine::OpenGL
{

class Renderer : public Engine::Renderer
{
public:
    explicit Renderer(Window &window);
    ~Renderer() override;

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

    [[nodiscard]] inline RendererAPI GetAPI() const override { return RendererAPI::RendererAPI_OpenGL; };

private:
#if defined(PLATFORM_WINDOWS)
    HDC m_deviceContext;
    HGLRC m_renderingContext;
#elif defined(PLATFORM_LINUX)
    GLXContext m_context;
#elif defined(PLATFORM_MACOS)
#error "MacOS is not supported yet"
#endif
};

}