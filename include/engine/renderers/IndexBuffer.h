//
// Created by Killian on 17/04/2023.
//
#pragma once

#include "engine/renderers/Mapping.h"

namespace Engine
{

enum class IndexBufferUsage : uint8_t
{
    IndexBufferUsage_Static,
    IndexBufferUsage_Dynamic,
    IndexBufferUsage_Stream
};

class IndexBuffer
{
public:
    using MappingType = uint32_t;

    IndexBuffer() = default;
    virtual ~IndexBuffer() = default;

    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer(IndexBuffer&& other) = delete;

    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) = delete;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetData(const uint32_t* data, size_t count) = 0;
    virtual void UpdateData(const uint32_t* data, size_t count, size_t offset) = 0;
    [[nodiscard]] virtual Mapping<IndexBuffer, uint32_t> Map(size_t offset, size_t count) = 0;

    [[nodiscard]] virtual size_t GetCount() = 0;

protected:
    friend Mapping<IndexBuffer, uint32_t>;

    virtual void Unmap() = 0;
};

}