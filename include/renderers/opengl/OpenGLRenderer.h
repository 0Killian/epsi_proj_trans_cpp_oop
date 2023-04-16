//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "renderers/Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
    explicit OpenGLRenderer(Window& window);

    ~OpenGLRenderer() override;

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

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