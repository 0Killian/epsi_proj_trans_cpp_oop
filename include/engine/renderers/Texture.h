//
// Created by Killian on 19/04/2023.
//

#pragma once

namespace Engine
{

class Texture
{
public:
    Texture() = default;
    virtual ~Texture() = default;

    Texture(const Texture& other) = delete;
    Texture(Texture&& other) = delete;
    Texture& operator=(const Texture& other) = delete;
    Texture& operator=(Texture&& other) = delete;

    virtual void LoadFromFile(const std::string_view& path) = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

}
