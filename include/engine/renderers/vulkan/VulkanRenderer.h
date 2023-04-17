//
// Created by Killian on 16/04/2023.
//

#pragma once

#include "engine/renderers/Renderer.h"

namespace Vulkan
{

class Renderer : public ::Renderer
{
public:
    Renderer(Window &window);

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

    [[nodiscard]] inline RendererAPI GetAPI() const override { return RendererAPI::RendererAPI_Vulkan; };
};

}