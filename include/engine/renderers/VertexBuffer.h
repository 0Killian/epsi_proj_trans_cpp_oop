//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "VertexBufferLayout.h"
#include "Renderer.h"

template <IsVertexBufferElement T>
class VertexBuffer
{
public:
    class Mapping
    {
    public:
        Mapping(VertexBuffer& buffer, T* data) : m_buffer(buffer), m_data(data)
        {
            m_buffer.UpdateMappingPointer(this);
        }

        Mapping(const Mapping& other) = delete;
        Mapping(Mapping&& other) noexcept
            : m_buffer(other.m_buffer), m_data(other.m_data)
        {
            other.m_data = nullptr;
        }

        Mapping& operator=(const Mapping& other) = delete;
        Mapping& operator=(Mapping&& other) noexcept
        {
            m_buffer = other.m_buffer;
            m_data = other.m_data;
            other.m_data = nullptr;

            m_buffer.UpdateMappingPointer(this);
        }

        ~Mapping()
        {
            if(m_data != nullptr)
                m_buffer.Unmap();
        }

        T& operator[](size_t index)
        {
            return m_data[index];
        }

        T* operator->()
        {
            return m_data;
        }

        T& operator*()
        {
            return *m_data;
        }

        T* operator&()
        {
            return m_data;
        }

    private:
        VertexBuffer& m_buffer;
        T* m_data;
    };

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
    [[nodiscard]] virtual Mapping Map(size_t offset, size_t count) = 0;

protected:
    friend Mapping;

    virtual void UpdateMappingPointer(Mapping* mapping) = 0;
    virtual void Unmap() = 0;
};