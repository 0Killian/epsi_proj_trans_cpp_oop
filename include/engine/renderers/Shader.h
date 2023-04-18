//
// Created by Killian on 18/04/2023.
//

#pragma once

class Shader
{
public:
    enum class Type : uint8_t
    {
        Vertex,
        TessellationControl,
        TessellationEvaluation,
        Geometry,
        Fragment,
        Compute
    };

    virtual ~Shader() = default;

    virtual void LoadFromFile(const std::string& path) = 0;
};
