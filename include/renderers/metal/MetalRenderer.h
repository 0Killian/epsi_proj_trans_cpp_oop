//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_MACOS
#pragma once

#include "renderers/Renderer.h"

class MetalRenderer : public Renderer
{
public:
    MetalRenderer(Window& window);

    void SetVSync(bool vsync) override;
};
#endif