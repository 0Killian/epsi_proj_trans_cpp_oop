//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_MACOS

#include "renderers/metal/MetalRenderer.h"

namespace Engine::Metal
{

Renderer::Renderer(Engine::Window& window)
    : Engine::Renderer(window)
{

}

void Renderer::SetVSync(bool vsync)
{

}

void Renderer::Clear(Color color)
{

}

void Renderer::SwapBuffers()
{

}

}

#endif