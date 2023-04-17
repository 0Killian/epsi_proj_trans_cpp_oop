//
// Created by Killian on 16/04/2023.
//

#ifdef PLATFORM_WIN32
#pragma once

#include "engine/renderers/Renderer.h"

namespace DirectX
{

class Renderer : public ::Renderer
{
public:
    Renderer(Window &window);

    void SetVSync(bool vsync) override;
    void Clear(Color color) override;
    void SwapBuffers() override;

    [[nodiscard]] inline RendererAPI GetAPI() const override { return RendererAPI::RendererAPI_DirectX; };
};

}

#endif
