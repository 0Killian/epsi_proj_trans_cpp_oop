//
// Created by Killian on 20/04/2023.
//

#pragma once

#include "engine/renderers/Pipeline.h"

namespace Engine
{

class Pipeline3D
{
public:
    struct Vertex
    {
        Vector3<float> position;
        Color color;

        static constexpr VertexBufferLayout GetLayout()
        {
            return {
                { "aWorldPosition", ShaderBaseType::ShaderBaseType_Float3 },
                { "aColor", ShaderBaseType::ShaderBaseType_Float4 }
            };
        }
    };

    Pipeline3D(const std::shared_ptr<Renderer>& renderer, std::shared_ptr<Pipeline> pipeline);

    Pipeline3D(const Pipeline3D& other) = delete;
    Pipeline3D(Pipeline3D&& other) = delete;
    Pipeline3D& operator=(const Pipeline3D& other) = delete;
    Pipeline3D& operator=(Pipeline3D&& other) = delete;

    virtual ~Pipeline3D() = default;

    inline void Bind() { m_pipeline->Bind(); }
    inline void Unbind() { m_pipeline->Unbind(); }

    inline void SetViewport(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size)
    { m_pipeline->SetViewport(pos, size); }
    inline void SetScissor(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size)
    { m_pipeline->SetScissor(pos, size); }

    void SetProjectionMatrix(const Matrix4<float>& matrix);
    void SetViewMatrix(const Matrix4<float>& matrix);
    void SetModelMatrix(const Matrix4<float>& matrix);

    inline void SetVertexBuffer(const std::shared_ptr<VertexBuffer<Vertex>>& vertexBuffer)
    { m_pipeline->SetVertexBuffer(vertexBuffer); }
    inline void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    { m_pipeline->SetIndexBuffer(indexBuffer); }

    inline void Render() { m_pipeline->Render(); }

private:
    std::shared_ptr<Pipeline> m_pipeline;

    Matrix4<float> m_projectionMatrix;
    Matrix4<float> m_viewMatrix;
    Matrix4<float> m_modelMatrix;
};

}
