//
// Created by Killian on 18/04/2023.
//

#pragma once

#include "engine/math/Vector2.h"
#include "engine/renderers/Shader.h"
#include "engine/renderers/VertexBuffer.h"
#include "engine/renderers/IndexBuffer.h"

namespace Engine
{

enum class PrimitiveTopology : uint8_t
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
};


// TODO: support multiple vertex types (reflection ?, vertex buffer base ?)
template<typename T>
class Pipeline
{
public:
    virtual ~Pipeline() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetViewport(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size) = 0;
    virtual void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    virtual void SetVertexShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetTessellationControlShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetTessellationEvaluationShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetGeometryShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetFragmentShader(const std::shared_ptr<Shader>& shader) = 0;
    virtual void SetComputeShader(const std::shared_ptr<Shader>& shader) = 0;

    virtual void Render() = 0;

    inline void SetVertexBuffer(const std::shared_ptr<VertexBuffer<T>>& vertexBuffer)
    {
        m_vertexBuffer = vertexBuffer;
        SetVertexBuffer();
    }

    inline void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        m_indexBuffer = indexBuffer;
        SetIndexBuffer();
    }

    [[nodiscard]] inline const std::shared_ptr<VertexBuffer<T>>& GetVertexBuffer() const { return m_vertexBuffer; }
    [[nodiscard]] inline const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }

protected:
    virtual void SetVertexBuffer() = 0;
    virtual void SetIndexBuffer() = 0;

    std::shared_ptr<VertexBuffer<T>> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
};

}