//
// Created by Killian on 17/04/2023.
//
#pragma once

#include "engine/renderers/Mapping.h"


class IndexBuffer
{
public:
    enum class Usage : uint8_t
    {
        Static,
        Dynamic,
        Stream
    };

    virtual ~IndexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetData(const uint32_t* data, size_t count) = 0;
    virtual void UpdateData(const uint32_t* data, size_t count, size_t offset) = 0;
    [[nodiscard]] virtual Mapping<::IndexBuffer, uint32_t> Map(size_t offset, size_t count) = 0;

    [[nodiscard]] virtual size_t GetCount() = 0;

protected:
    friend Mapping<::IndexBuffer, uint32_t>;

    virtual Mapping<::IndexBuffer, uint32_t>* UpdateMappingPointer(Mapping<IndexBuffer, uint32_t>* mapping) = 0;
    virtual Mapping<::IndexBuffer, uint32_t>* Unmap() = 0;
};