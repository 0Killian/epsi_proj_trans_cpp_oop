//
// Created by Killian on 18/04/2023.
//

#pragma once

#include <spirv_glsl.hpp>
#include "engine/renderers/Shader.h"

namespace OpenGL
{

class Shader : public ::Shader
{
public:
    explicit Shader(::Shader::Type type);
    ~Shader() override;

    void LoadFromFile(const std::string& path) override;

protected:
    template <typename T>
    friend class Pipeline;

    [[nodiscard]] inline uint32_t GetId() const { return m_id; }
    [[nodiscard]] inline ::Shader::Type GetType() const { return m_type; }

private:
    void ConvertSampledImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertSeparateImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertStorageImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertSeparateSamplers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertUniformBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertPushConstantBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertSubpassInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertStageInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertStageOutputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertStorageBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertAccelerationStructures(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertShaderRecordBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertAtomicCounters(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources);
    void ConvertBuiltinInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::BuiltInResource>& resources);
    void ConvertBuiltinOutputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::BuiltInResource>& resources);

    uint32_t m_id = 0;
    ::Shader::Type m_type;
};

}