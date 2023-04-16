//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_WIN32
#include "renderers/directx/DirectXRenderer.h"

DirectXRenderer::DirectXRenderer(Window &window)
    : Renderer(window)
{

}

void DirectXRenderer::SetVSync(bool vsync)
{

}

#endif