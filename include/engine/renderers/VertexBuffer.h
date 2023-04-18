//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "engine/renderers/Mapping.h"

template <IsVertexBufferElement T>
class VertexBuffer
{
public:
    enum class Usage : uint8_t
    {
        Static,
        Dynamic,
        Stream
    };

    virtual ~VertexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetData(const T* data, size_t count) = 0;
    virtual void UpdateData(const T* data, size_t count, size_t offset) = 0;
    [[nodiscard]] virtual Mapping<VertexBuffer, T> Map(size_t offset, size_t count) = 0;

protected:
    friend Mapping<VertexBuffer, T>;

    virtual void UpdateMappingPointer(Mapping<VertexBuffer, T>* mapping) = 0;
    virtual void Unmap() = 0;
};