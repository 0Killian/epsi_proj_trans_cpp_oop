//
// Created by Killian on 20/04/2023.
//

#pragma once

#include "engine/renderers/Pipeline.h"

namespace Engine
{

class Pipeline3DTextured
{
public:
    struct Vertex
    {
        Vector3<float> position;
        Vector2<float> texCoord;

        static constexpr VertexBufferLayout GetLayout()
        {
            return {
                { "aWorldPosition", ShaderBaseType::ShaderBaseType_Float3 },
                { "aTexCoord", ShaderBaseType::ShaderBaseType_Float2 }
            };
        }
    };

    Pipeline3DTextured(const std::shared_ptr<Renderer>& renderer, std::shared_ptr<Pipeline> pipeline);

    Pipeline3DTextured(const Pipeline3DTextured& other) = delete;
    Pipeline3DTextured(Pipeline3DTextured&& other) = delete;
    Pipeline3DTextured& operator=(const Pipeline3DTextured& other) = delete;
    Pipeline3DTextured& operator=(Pipeline3DTextured&& other) = delete;

    virtual ~Pipeline3DTextured() = default;

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

    inline void SetTexture(const std::shared_ptr<Texture>& texture)
    { m_pipeline->SetUniform("uTexture", texture); }

    inline void Render() { m_pipeline->Render(); }

protected:
    std::shared_ptr<Pipeline> m_pipeline;

    Matrix4<float> m_projectionMatrix;
    Matrix4<float> m_viewMatrix;
    Matrix4<float> m_modelMatrix;
};

}
