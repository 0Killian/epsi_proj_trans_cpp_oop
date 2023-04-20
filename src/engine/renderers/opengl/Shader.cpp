//
// Created by Killian on 18/04/2023.
//
#include "engine/renderers/opengl/Shader.h"
#include "engine/glad/glad.h"
#include <fstream>

namespace Engine::OpenGL
{

Shader::Shader(ShaderType type) : m_type(type)
{
    switch (m_type)
    {
    case ShaderType::ShaderType_Vertex:m_id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case ShaderType::ShaderType_TessellationControl:m_id = glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
    case ShaderType::ShaderType_TessellationEvaluation:m_id = glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
    case ShaderType::ShaderType_Geometry:m_id = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    case ShaderType::ShaderType_Fragment:m_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case ShaderType::ShaderType_Compute:m_id = glCreateShader(GL_COMPUTE_SHADER);
        break;
    }
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

void Shader::LoadFromFile(const std::string &path)
{
    // Load the SPIR-V binary
    std::vector<uint32_t> spirvBinary;

    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    size_t fileSize = (size_t)file.tellg();
    spirvBinary.resize(fileSize / sizeof(uint32_t) + (fileSize % sizeof(uint32_t) != 0));

    file.seekg(0);
    file.read(reinterpret_cast<char*>(spirvBinary.data()), static_cast<std::streamsize>(fileSize));
    file.close();

    // Compile the SPIR-V binary
    spirv_cross::CompilerGLSL compiler(spirvBinary);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    // Convert its resources
    ConvertSampledImages(compiler, resources.sampled_images);
    ConvertSeparateImages(compiler, resources.separate_images);
    ConvertStorageImages(compiler, resources.storage_images);
    ConvertSeparateSamplers(compiler, resources.separate_samplers);
    ConvertUniformBuffers(compiler, resources.uniform_buffers);
    ConvertPushConstantBuffers(compiler, resources.push_constant_buffers);
    ConvertSubpassInputs(compiler, resources.subpass_inputs);
    ConvertStageInputs(compiler, resources.stage_inputs);
    ConvertStageOutputs(compiler, resources.stage_outputs);
    ConvertStorageBuffers(compiler, resources.storage_buffers);
    ConvertAccelerationStructures(compiler, resources.acceleration_structures);
    ConvertShaderRecordBuffers(compiler, resources.shader_record_buffers);
    ConvertAtomicCounters(compiler, resources.atomic_counters);
    ConvertBuiltinInputs(compiler, resources.builtin_inputs);
    ConvertBuiltinOutputs(compiler, resources.builtin_outputs);

    // Define compiler options
    spirv_cross::CompilerGLSL::Options options;
    options.version = 450;
    options.es = false;
    compiler.set_common_options(options);

    // Transpile to GLSL, ready to give to GL driver
    std::string source = compiler.compile();

    // Compile GLSL shader
    const char* sourcePtr = source.c_str();
    glShaderSource(m_id, 1, &sourcePtr, nullptr);
    glCompileShader(m_id);

    // Check if compilation was successful
    GLint success = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint length = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);

        std::vector<char> message(static_cast<size_t>(length));
        glGetShaderInfoLog(m_id, length, &length, message.data());

        throw std::runtime_error(std::string("Failed to compile shader: ") + message.data());
    }
}

void Shader::ConvertSampledImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: sampler2d
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertSeparateImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: texture2d/samplerBuffer
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertStorageImages(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: image2d/imageBuffer
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertSeparateSamplers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: sampler/samplerShadow
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertUniformBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: uniform UBO {}
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertPushConstantBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: layout(push_constant) uniform Push
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertSubpassInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: subpassInput
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertStageInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: in vec2 uv
    {
        compiler.unset_decoration(resource.id, spv::DecorationLocation);
    }
}

void Shader::ConvertStageOutputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: out vec4 color
    {
        compiler.unset_decoration(resource.id, spv::DecorationLocation);
    }
}

void Shader::ConvertStorageBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: buffer SSBO {}
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertAccelerationStructures(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // Ray tracing, GLSL: ?
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertShaderRecordBuffers(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources)
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertAtomicCounters(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::Resource>& resources)
{
    for(auto& resource : resources) // GLSL: ?
    {
        compiler.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertBuiltinInputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::BuiltInResource>& resources)
{
    for(auto& resource : resources) // GLSL: ?
    {
        compiler.unset_decoration(resource.resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.resource.id, spv::DecorationBinding);
    }
}

void Shader::ConvertBuiltinOutputs(spirv_cross::CompilerGLSL& compiler, spirv_cross::SmallVector<spirv_cross::BuiltInResource>& resources)
{
    for(auto& resource : resources) // GLSL: ?
    {
        compiler.unset_decoration(resource.resource.id, spv::DecorationDescriptorSet);
        compiler.unset_decoration(resource.resource.id, spv::DecorationBinding);
    }
}


}