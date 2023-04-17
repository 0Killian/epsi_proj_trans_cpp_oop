//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_WIN32
#include "engine/renderers/directx/DirectXRenderer.h"

namespace DirectX
{

Renderer::Renderer(Window &window)
    : ::Renderer(window)
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