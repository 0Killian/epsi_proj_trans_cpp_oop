//
// Created by Killian on 17/04/2023.
//
#include "engine/renderers/opengl/IndexBuffer.h"

namespace OpenGL
{

IndexBuffer::IndexBuffer(::IndexBuffer::Usage usage)
    : m_usage(usage)
{
    glGenBuffers(1, &m_id);

#ifdef DEBUG
    if(m_id == 0)
        throw std::runtime_error("Failed to create OpenGL index buffer.");
#endif
}

IndexBuffer::IndexBuffer(::IndexBuffer::Usage usage, const uint32_t* indices, size_t count)
    : IndexBuffer(usage)
{
    SetData(indices, count);
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other)
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

    m_count = other.m_count;
    m_usage = other.m_usage;

    return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if(this == &other)
        return *this;

    m_id = other.m_id;
    m_count = other.m_count;
    m_usage = other.m_usage;

    other.m_id = 0;

    return *this;
}

std::vector<std::shared_ptr<::IndexBuffer>> IndexBuffer::CreateIndexBuffers(::IndexBuffer::Usage usage, size_t count)
{
    std::vector<uint32_t> ids(count);
    glGenBuffers(static_cast<GLsizei>(count), ids.data());
    std::vector<std::shared_ptr<::IndexBuffer>> buffers(count);
    std::transform(ids.begin(), ids.end(), buffers.begin(), [usage](uint32_t id)
    { return std::shared_ptr<IndexBuffer>(new IndexBuffer(usage, id)); });

    return buffers;
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::Unbind()
{
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(const uint32_t* indices, size_t count)
{
    Bind();

    switch(m_usage)
    {
        case Usage::Static:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
        break;
        case Usage::Dynamic:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_DYNAMIC_DRAW);
        break;
        case Usage::Stream:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_STREAM_DRAW);
        break;
    }

    m_count = count;
}

void IndexBuffer::UpdateData(const uint32_t* data, size_t count, size_t offset)
{
    Bind();

#ifdef DEBUG
    if(offset + count > m_count)
        throw std::runtime_error("Trying to update data outside of the OpenGL index buffer.");

    if(m_mapping != nullptr)
        throw std::runtime_error("Trying to update an OpenGL index buffer that is still mapped.");

    if(data == nullptr)
        throw std::runtime_error("Trying to update an OpenGL index buffer with a null pointer.");
#endif

    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLintptr>(offset * sizeof(uint32_t)),
        static_cast<GLintptr>(count * sizeof(uint32_t)),
        data
    );
}

[[nodiscard]] ::IndexBuffer::Mapping IndexBuffer::Map(size_t offset, size_t count)
{
    Bind();

    if(count == 0)
        count = m_count;

#ifdef DEBUG
    if(offset + count > m_count)
        throw std::runtime_error("Trying to map data outside of the OpenGL index buffer.");

    if(m_mapping != nullptr)
        throw std::runtime_error("Trying to map an OpenGL index buffer that is already mapped.");
#endif

    auto* data = reinterpret_cast<uint32_t*>(glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLintptr>(offset * sizeof(uint32_t)),
        static_cast<GLintptr>(count * sizeof(uint32_t)),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
    ));

    return { *this, data };
}

IndexBuffer::~IndexBuffer()
{
#ifdef DEBUG
    if(m_id == 0)
        spdlog::error("Trying to delete an already deleted OpenGL VertexBuffer");

    if(m_mapping != nullptr)
        spdlog::error("Trying to delete an OpenGL VertexBuffer that is still mapped");
#endif
    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::Unmap()
{
    Bind();
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    m_mapping = nullptr;
}

IndexBuffer::IndexBuffer(::IndexBuffer::Usage usage, uint32_t id)
    : m_usage(usage), m_id(id)
{
#ifdef DEBUG
    if(m_id == 0)
        throw std::runtime_error("Trying to create an OpenGL index buffer with an id of 0.");
#endif
}

}