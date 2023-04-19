//
// Created by Killian on 18/04/2023.
//

#pragma once

namespace Engine
{

enum class ShaderType : uint8_t
{
    ShaderType_Vertex,
    ShaderType_TessellationControl,
    ShaderType_TessellationEvaluation,
    ShaderType_Geometry,
    ShaderType_Fragment,
    ShaderType_Compute
};

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void LoadFromFile(const std::string& path) = 0;
};

}
