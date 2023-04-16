//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "renderers/Renderer.h"

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(Window& window);

    void SetVSync(bool vsync) override;
};