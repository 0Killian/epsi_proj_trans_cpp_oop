//
// Created by Killian on 20/04/2023.
//

#pragma once
#include "VertexBufferLayout.h"

namespace Engine
{

enum class VertexBufferUsage : uint8_t
{
    VertexBufferUsage_Static,
    VertexBufferUsage_Dynamic,
    VertexBufferUsage_Stream
};

class VertexBufferBase
{
public:
    virtual ~VertexBufferBase() = default;

    virtual void SetData(const void* data, size_t size) = 0;
    virtual void UpdateData(const void* data, size_t size, size_t offset) = 0;

    [[nodiscard]] virtual void* Map(size_t offset, size_t size) = 0;

protected:
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Unmap() = 0;

    template <IsVertexBufferElement T>
    friend class VertexBuffer;
};

}