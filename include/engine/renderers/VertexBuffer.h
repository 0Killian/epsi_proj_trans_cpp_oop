//
// Created by Killian on 17/04/2023.
//

#pragma once

#include <utility>

#include "Renderer.h"
#include "engine/renderers/Mapping.h"
#include "VertexBufferBase.h"

namespace Engine
{

template<IsVertexBufferElement T>
class VertexBuffer
{
public:
    explicit VertexBuffer(std::shared_ptr<VertexBufferBase> base) : m_base(std::move(base)) {}

    inline void Bind() { m_base->Bind(); }
    inline void Unbind() { m_base->Unbind(); }

    inline void SetData(const T* data, size_t count) { m_base->SetData(data, count * sizeof(T)); }
    inline void UpdateData(const T* data, size_t count, size_t offset)
    { m_base->UpdateData(data, count * sizeof(T), offset * sizeof(T)); }

    [[nodiscard]] virtual Mapping<VertexBuffer, T> Map(size_t offset, size_t count)
    { return Mapping<VertexBuffer, T>(this, reinterpret_cast<T*>(m_base->Map(offset * sizeof(T), count * sizeof(T)))); }

protected:
    friend Mapping<VertexBuffer, T>;
    friend class Pipeline;

    void Unmap() { m_base->Unmap(); }

private:
    std::shared_ptr<VertexBufferBase> m_base;
};

}