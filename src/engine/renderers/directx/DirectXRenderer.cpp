//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_WINDOWS

#include "engine/renderers/directx/DirectXRenderer.h"


namespace Engine::DirectX
{

Renderer::Renderer(Engine::Window &window)
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