//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_MACOS
#pragma once

#include "renderers/Renderer.h"

namespace Engine::Metal
{

class Renderer : public Engine::Renderer
{
public:
    Renderer(Window& window);

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

    [[nodiscard]] inline RendererAPI GetAPI() const override { return RendererAPI::RendererAPI_Metal; };
};

}
#endif