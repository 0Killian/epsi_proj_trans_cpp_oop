//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/VertexBuffer.h"
#include "engine/glad/glad.h"
#include "engine/glad/glad.h"

namespace OpenGL
{

GLenum ShaderDataTypeToOpenGLBaseType(VertexBufferLayout::Type type);

template <IsVertexBufferElement T>
class VertexBuffer : public ::VertexBuffer<T>
{
public:
    explicit VertexBuffer(typename ::VertexBuffer<T>::Usage usage) : m_usage(usage)
    {
        glGenBuffers(1, &m_id);
#ifdef DEBUG
        if(m_id == 0)
            throw std::runtime_error("Failed to create OpenGL VertexBuffer");
#endif
    };

    VertexBuffer(const VertexBuffer& other)
    {
        operator=(other);
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

    static std::vector<std::shared_ptr<::VertexBuffer<T>>> CreateVertexBuffers(typename ::VertexBuffer<T>::Usage usage, int count)
    {
        // TODO: profile multiple vertex buffer creation vs one by one
        std::vector<std::shared_ptr<::VertexBuffer<T>>> buffers(count);
        std::vector<uint32_t> ids(count);
        glGenBuffers(count, ids.data());
        std::transform(ids.begin(), ids.end(), buffers.begin(), [&](uint32_t id)
            { return std::shared_ptr<VertexBuffer>(new VertexBuffer(usage, id)); });

        return buffers;
    }

    void Bind() override
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void Unbind() override
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetData(const T* data, size_t count) override
    {
        Bind();
        switch(m_usage)
        {
        case ::VertexBuffer<T>::Usage::Static:
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data, GL_STATIC_DRAW);
            break;
        case ::VertexBuffer<T>::Usage::Dynamic:
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data, GL_DYNAMIC_DRAW);
            break;
        case ::VertexBuffer<T>::Usage::Stream:
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), data, GL_STREAM_DRAW);
            break;
        }
    }

    void UpdateData(const T* data, size_t count, size_t offset) override
    {
        if(count == 0)
            return;

        Bind();

#ifdef DEBUG
        GLint bufferSize;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

        if((offset + count) * sizeof(T) > bufferSize)
            throw std::runtime_error("Trying to update data outside of the buffer");

        if(m_mapping != nullptr)
            throw std::runtime_error("Trying to update data while the buffer is mapped");

        if(data == nullptr)
            throw std::runtime_error("Trying to update data with a null pointer");
#endif

        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(T), count * sizeof(T), data);
    }

    typename ::VertexBuffer<T>::Mapping Map(size_t offset, size_t count) override
    {
        Bind();

        GLint bufferSize;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

        if(count == 0)
        {
            count = bufferSize / sizeof(T);
        }

#ifdef DEBUG
        if((offset + count) * sizeof(T) > bufferSize)
            throw std::runtime_error("Trying to update data outside of the buffer");

        if(m_mapping != nullptr)
            throw std::runtime_error("Trying to update data while the buffer is mapped");
#endif

        T* data = reinterpret_cast<T*>(glMapBufferRange(
            GL_ARRAY_BUFFER,
            static_cast<GLintptr>(offset * sizeof(T)),
            static_cast<GLint>(count * sizeof(T)),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT)
        );

        return ::VertexBuffer<T>::Mapping(*this, data);
    }

    ~VertexBuffer() override
    {
#ifdef DEBUG
        if(m_id == 0)
            spdlog::error("Trying to delete an already deleted OpenGL VertexBuffer");

        if(m_mapping != nullptr)
            spdlog::error("Trying to delete an OpenGL VertexBuffer that is still mapped");
#endif
        glDeleteBuffers(1, &m_id);
    }

protected:
    void UpdateMappingPointer(typename ::VertexBuffer<T>::Mapping* ptr) override
    {
        m_mapping = ptr;
    }

    void Unmap() override
    {
        Bind();
        glUnmapBuffer(GL_ARRAY_BUFFER);
        m_mapping = nullptr;
    }

private:
    VertexBuffer(typename ::VertexBuffer<T>::Usage usage, uint32_t id)
        : m_id(id), m_usage(usage)
    {}

    uint32_t m_id = 0;
    typename ::VertexBuffer<T>::Usage m_usage = ::VertexBuffer<T>::Usage::Static;
    typename ::VertexBuffer<T>::Mapping* m_mapping = 0;
};

}