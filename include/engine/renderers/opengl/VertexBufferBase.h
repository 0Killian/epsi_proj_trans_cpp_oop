//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/VertexBufferBase.h"
#include "engine/renderers/VertexBufferLayout.h"
#include "engine/glad/glad.h"

namespace Engine::OpenGL
{

GLenum ShaderBaseTypeToOpenGLBaseType(ShaderBaseType type);
ShaderBaseType OpenGLBaseTypeToShaderBaseType(GLenum type);
std::string ShaderBaseTypeToString(ShaderBaseType type);

class VertexBufferBase : public Engine::VertexBufferBase
{
public:
    explicit VertexBufferBase(VertexBufferUsage usage);

    VertexBufferBase(const VertexBufferBase& other) = delete;
    VertexBufferBase(VertexBufferBase&& other) noexcept = delete;

    VertexBufferBase& operator=(const VertexBufferBase& other) = delete;
    VertexBufferBase& operator=(VertexBufferBase&& other) noexcept = delete;

    static std::vector<std::shared_ptr<Engine::VertexBufferBase>> CreateVertexBuffers(VertexBufferUsage usage, int count);

    void Bind() override;
    void Unbind() override;

    void SetData(const void* data, size_t size) override;
    void UpdateData(const void* data, size_t size, size_t offset) override;

    void* Map(size_t offset, size_t size) override;

    ~VertexBufferBase() override;

protected:
    void Unmap() override;

private:
    VertexBufferBase(VertexBufferUsage usage, uint32_t id);

    uint32_t m_id = 0;
    VertexBufferUsage m_usage = VertexBufferUsage::VertexBufferUsage_Static;
    bool m_mapped = false;
};

}