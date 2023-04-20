//
// Created by Killian on 19/04/2023.
//

#pragma once

#include "engine/renderers/Texture.h"

namespace Engine::OpenGL
{

class Texture : public Engine::Texture
{
public:
    Texture();
    ~Texture() override;

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    static std::vector<std::shared_ptr<Engine::Texture>> CreateTextures(size_t count);

    void LoadFromFile(const std::string_view& path) override;

    void Bind() override;
    void Unbind() override;

private:
    explicit Texture(uint32_t id);

    uint32_t m_id = 0;

    static uint32_t s_boundId;
};

}