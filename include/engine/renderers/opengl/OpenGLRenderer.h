//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/renderers/Renderer.h"

namespace OpenGL
{

class Renderer : public ::Renderer
{
public:
    explicit Renderer(Window &window);
    ~Renderer() override;

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

    [[nodiscard]] inline RendererAPI GetAPI() const override { return RendererAPI::RendererAPI_OpenGL; };

private:
#ifdef PLATFORM_WIN32
    HDC m_deviceContext;
    HGLRC m_renderingContext;
#elif PLATFORM_LINUX
    GLXContext m_renderingContext;
#elif PLATFORM_MACOS
#error "MacOS is not supported yet"
#endif
};

}