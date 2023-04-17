//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/VertexBuffer.h"
#include "engine/glad/glad.h"

namespace OpenGL
{

template <IsVertexBufferElement T>
class VertexBuffer : ::VertexBuffer<T>
{
public:
    explicit VertexBuffer(::VertexBuffer<T>::Usage usage) : m_usage(usage)
    {
        glGenBuffers(1, &m_id);
#ifdef DEBUG
        if(m_id == 0)
            throw std::runtime_error("Failed to create OpenGL VertexBuffer");
#endif
    };

    VertexBuffer(::VertexBuffer<T>::Usage usage, uint32_t id) : m_id(id)
    {
#ifdef DEBUG
        if(m_id == 0)
            throw std::runtime_error("Failed to create OpenGL VertexBuffer");
#endif

        m_usage = usage;
    }

    VertexBuffer(const VertexBuffer& other)
    {
        *this = std::copy(other);
    }

    VertexBuffer(VertexBuffer&& other) noexcept
    {
        *this = std::move(other);
    }

    VertexBuffer& operator=(const VertexBuffer& other)
    {
        if(this == &other)
            return *this;

        if(m_id == 0)
            glGenBuffers(1, &m_id);

        glBindBuffer(GL_COPY_READ_BUFFER, m_id);
        glBindBuffer(GL_COPY_WRITE_BUFFER, other.m_id);

        GLint size;
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

        m_usage = other.m_usage;

        return *this;
    }

    VertexBuffer& operator=(VertexBuffer&& other) noexcept
    {
        m_id = other.m_id;
        m_usage = other.m_usage;
        other.m_id = 0;
        return *this;
    }

    static std::vector<VertexBuffer> CreateVertexBuffers(::VertexBuffer<T>::Usage usage, int count)
    {
        // TODO: profile multiple vertex buffer creation vs one by one
        std::vector<VertexBuffer> buffers(count);
        std::vector<uint32_t> ids(count);
        glGenBuffers(count, ids.data());
        std::transform(ids.begin(), ids.end(), buffers.begin(), [&](uint32_t id) { return VertexBuffer(usage, id); });
        return buffers;
    }

    void Bind() override
    {
        if (m_id != s_bound_id)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id);
            s_bound_id = m_id;
        }
    }

    void Unbind() override
    {
        if(s_bound_id != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            s_bound_id = 0;
        }
    }

    void SetData(const T* data, size_t size) override
    {
        Bind();
        switch(m_usage)
        {
        case ::VertexBuffer<T>::Usage::Static:
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            break;
        case ::VertexBuffer<T>::Usage::Dynamic:
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
            break;
        case ::VertexBuffer<T>::Usage::Stream:
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
            break;
        }
    }

    void UpdateData(const T* data, size_t size, size_t offset) override
    {
        Bind();

#ifdef DEBUG
        GLint bufferSize;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

        if(offset + size > bufferSize)
            throw std::runtime_error("Trying to update data outside of the buffer");

        if(offset % sizeof(T) != 0)
            throw std::runtime_error("Trying to update data at an unaligned offset");

        if(size % sizeof(T) != 0)
            throw std::runtime_error("Trying to update data with an unaligned size");

        if(data == nullptr)
            throw std::runtime_error("Trying to update data with a null pointer");
#endif

        if(size == 0)
            return;

        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    ::VertexBuffer<T>::Mapping Map(size_t offset, size_t size) override
    {
        Bind();

        if(size == 0)
        {
            GLint bufferSize;
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
            size = bufferSize;
        }

        T* data = reinterpret_cast<T*>(glMapBufferRange(
            GL_ARRAY_BUFFER,
            static_cast<GLintptr>(offset),
            static_cast<GLint>(size),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT)
        );

        return ::VertexBuffer<T>::Mapping(data, size);
    }

protected:
    void UpdateMappingPointer(::VertexBuffer<T>::Mapping* ptr) override
    {
        m_mapping = ptr;
    }

    void Unmap() override
    {
        Bind();
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    ~VertexBuffer() override
    {
#ifdef DEBUG
        if(m_id == 0)
            throw std::runtime_error("Trying to delete an already deleted OpenGL VertexBuffer");

        if(m_mapping != nullptr)
            throw std::runtime_error("Trying to delete an OpenGL VertexBuffer that is still mapped");
#endif
        glDeleteBuffers(1, &m_id);

        if(m_id == s_bound_id)
            s_bound_id = 0;
    }

private:
    uint32_t m_id = 0;
    ::VertexBuffer<T>::Usage m_usage;
    ::VertexBuffer<T>::Mapping* m_mapping;

    static uint32_t s_bound_id;
};

}