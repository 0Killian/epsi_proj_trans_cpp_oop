//
// Created by Killian on 19/04/2023.
//

#pragma once

namespace Engine
{

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void LoadFromFile(const std::string_view& path) = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

}
