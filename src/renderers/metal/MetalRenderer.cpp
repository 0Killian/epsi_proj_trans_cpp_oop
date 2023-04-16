//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_MACOS
#include "renderers/metal/MetalRenderer.h"

MetalRenderer::MetalRenderer(Window& window)
    : Renderer(window)
{

}

void MetalRenderer::SetVSync(bool vsync)
{

}

#endif