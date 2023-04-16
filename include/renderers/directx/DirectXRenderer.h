//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_WIN32
#pragma once

#include "renderers/Renderer.h"

class DirectXRenderer : public Renderer
{
public:
    DirectXRenderer(Window& window);

    void SetVSync(bool vsync) override;
};

#endif
