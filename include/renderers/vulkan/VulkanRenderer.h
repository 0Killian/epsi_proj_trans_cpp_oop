//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "renderers/Renderer.h"

class VulkanRenderer : public Renderer
{
public:
    VulkanRenderer(Window& window);

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;
};