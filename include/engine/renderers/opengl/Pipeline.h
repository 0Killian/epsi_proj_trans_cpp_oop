//
// Created by Killian on 18/04/2023.
//
#pragma once

#include "engine/renderers/opengl/VertexBufferBase.h"
#include "engine/renderers/opengl/Shader.h"
#include "engine/renderers/Pipeline.h"
#include "engine/glad/glad.h"
#include "Texture.h"
#include "IndexBuffer.h"

namespace Engine::OpenGL
{

GLenum PrimitiveTopologyToOpenGLTopology(PrimitiveTopology topology);

class Pipeline : public Engine::Pipeline
{
public:
    Pipeline(
            Vector2<uint32_t> viewportPos,
            Vector2<uint32_t> viewportSize,
            Vector2<uint32_t> scissorsPos,
            Vector2<uint32_t> scissorsSize,
            PrimitiveTopology topology);

    Pipeline(const Pipeline& other) = delete;
    Pipeline(Pipeline&& other) = delete;
    Pipeline& operator=(const Pipeline& other) = delete;
    Pipeline& operator=(Pipeline&& other) = delete;

    ~Pipeline() override;

    void Bind() override;
    void Unbind() override;

    void SetViewport(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size) override;
    void SetScissor(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size) override;

    void SetVertexShader(const std::shared_ptr<Engine::Shader>& shader) override;
    void SetTessellationControlShader(const std::shared_ptr<Engine::Shader>& shader) override;
    void SetTessellationEvaluationShader(const std::shared_ptr<Engine::Shader>& shader) override;
    void SetGeometryShader(const std::shared_ptr<Engine::Shader>& shader) override;
    void SetFragmentShader(const std::shared_ptr<Engine::Shader>& shader) override;
    void SetComputeShader(const std::shared_ptr<Engine::Shader>& shader) override;

    void Render() override;

    void SetUniform(const std::string& name, float value) override;
    void SetUniform(const std::string& name, const Vector2<float>& value) override;
    void SetUniform(const std::string& name, const Vector3<float>& value) override;
    void SetUniform(const std::string& name, const Vector4<float>& value) override;
    void SetUniform(const std::string& name, const Matrix3<float>& value) override;
    void SetUniform(const std::string& name, const Matrix4<float>& value) override;
    void SetUniform(const std::string& name, int value) override;
    void SetUniform(const std::string& name, const Vector2<int>& value) override;
    void SetUniform(const std::string& name, const Vector3<int>& value) override;
    void SetUniform(const std::string& name, const Vector4<int>& value) override;
    void SetUniform(const std::string& name, bool value) override;
    void SetUniform(const std::string& name, const std::shared_ptr<Engine::Texture>& value) override;

protected:
    void SetVertexBuffer(
            const std::shared_ptr<Engine::VertexBufferBase>& vertexBufferBase,
            const VertexBufferLayout& layout) override;

    void SetIndexBuffer(const std::shared_ptr<Engine::IndexBuffer>& indexBuffer) override;

private:
    struct Uniform
    {
        std::string name;
        GLint location = -1;
        ShaderBaseType type = ShaderBaseType::ShaderBaseType_Unknown;
        uint32_t size = 0;
        bool isSet = false;
    };

    Uniform* FindUniform(const std::string& name);

    uint32_t m_programId = 0;
    uint32_t m_vertexArrayId = 0;

    Vector2<GLint> m_viewportPos = {};
    Vector2<GLint> m_viewportSize = {};
    Vector2<GLint> m_scissorsPos = {};
    Vector2<GLint> m_scissorsSize = {};

    std::shared_ptr<OpenGL::Shader> m_vertexShader;
    std::shared_ptr<OpenGL::Shader> m_tessellationControlShader;
    std::shared_ptr<OpenGL::Shader> m_tessellationEvaluationShader;
    std::shared_ptr<OpenGL::Shader> m_geometryShader;
    std::shared_ptr<OpenGL::Shader> m_fragmentShader;
    std::shared_ptr<OpenGL::Shader> m_computeShader;

    bool m_shouldLink = false;
    bool m_shouldUpdateBindings = false;
    bool m_linked = false;
    bool m_bound = false;

    GLint m_uniformCount = 0;
    GLint m_inputCount = 0;

    GLint m_maxUniformNameSize = 0;
    GLint m_maxInputNameSize = 0;

    PrimitiveTopology m_topology = PrimitiveTopology::Triangles;

    std::string m_uniformNameBuffer;
    std::string m_inputNameBuffer;

    std::vector<std::string> m_uniformNames;
    std::vector<std::string> m_inputNames;

    std::vector<Uniform> m_uniforms;
    std::shared_ptr<OpenGL::Texture> m_texture;
    std::vector<GLint> m_boundAttributeLocations;

    std::shared_ptr<OpenGL::VertexBufferBase> m_vertexBufferBase;
    std::shared_ptr<OpenGL::IndexBuffer> m_indexBuffer;
    VertexBufferLayout m_layout = {};
};

}