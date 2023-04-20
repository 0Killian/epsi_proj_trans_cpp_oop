//
// Created by Killian on 17/04/2023.
//
#include "engine/renderers/opengl/IndexBuffer.h"

namespace Engine::OpenGL
{

IndexBuffer::IndexBuffer(IndexBufferUsage usage) : m_usage(usage)
{
    glCreateBuffers(1, &m_id);

#ifdef DEBUG
    if(m_id == 0) throw std::runtime_error("Failed to create OpenGL index buffer.");
#endif
}

IndexBuffer::IndexBuffer(IndexBufferUsage usage, const uint32_t* indices, size_t count) : IndexBuffer(usage)
{
    SetData(indices, count);
}

std::vector<std::shared_ptr<Engine::IndexBuffer>> IndexBuffer::CreateIndexBuffers(IndexBufferUsage usage, size_t count)
{
    std::vector<uint32_t> ids(count);
    glGenBuffers(static_cast<GLsizei>(count), ids.data());
    std::vector<std::shared_ptr<Engine::IndexBuffer>> buffers(count);
    std::transform(ids.begin(), ids.end(), buffers.begin(), [usage](uint32_t id)
        { return std::shared_ptr<Engine::IndexBuffer>(new IndexBuffer(usage, id)); });

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
    switch(m_usage)
    {
        case IndexBufferUsage::IndexBufferUsage_Static:
            glNamedBufferData(m_id, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_STATIC_DRAW);
            break;
        case IndexBufferUsage::IndexBufferUsage_Dynamic:
            glNamedBufferData(m_id, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_DYNAMIC_DRAW);
            break;
        case IndexBufferUsage::IndexBufferUsage_Stream:
            glNamedBufferData(m_id, static_cast<GLintptr>(count * sizeof(uint32_t)), indices, GL_STREAM_DRAW);
            break;
    }

    m_count = count;
}

void IndexBuffer::UpdateData(const uint32_t* data, size_t count, size_t offset)
{
#ifdef DEBUG
    if(offset + count > m_count) throw std::runtime_error("Trying to update data outside of the OpenGL index buffer.");
    if(m_mapped) throw std::runtime_error("Trying to update an OpenGL index buffer that is still mapped.");
    if(data == nullptr) throw std::runtime_error("Trying to update an OpenGL index buffer with a null pointer.");
#endif

    glNamedBufferSubData(
        m_id,
        static_cast<GLintptr>(offset * sizeof(uint32_t)),
        static_cast<GLintptr>(count * sizeof(uint32_t)),
        data
    );
}

[[nodiscard]] Mapping<Engine::IndexBuffer, uint32_t> IndexBuffer::Map(size_t offset, size_t count)
{
    if(count == 0) count = m_count;

#ifdef DEBUG
    if(offset + count > m_count) throw std::runtime_error("Trying to map data outside of the OpenGL index buffer.");
    if(m_mapped) throw std::runtime_error("Trying to map an OpenGL index buffer that is already mapped.");
#endif

    auto* data = reinterpret_cast<uint32_t*>(glMapNamedBufferRange(
        m_id,
        static_cast<GLintptr>(offset * sizeof(uint32_t)),
        static_cast<GLintptr>(count * sizeof(uint32_t)),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
    ));

    return Mapping<OpenGL::IndexBuffer, uint32_t>(this, data);
}

IndexBuffer::~IndexBuffer()
{
#ifdef DEBUG
    if(m_id == 0) spdlog::error("Trying to delete an already deleted OpenGL VertexBuffer");
    if(m_mapped) spdlog::error("Trying to delete an OpenGL VertexBuffer that is still mapped");
#endif

    glDeleteBuffers(1, &m_id);
}

void IndexBuffer::Unmap()
{
    glUnmapNamedBuffer(m_id);
}

IndexBuffer::IndexBuffer(IndexBufferUsage usage, uint32_t id) : m_usage(usage), m_id(id)
{
#ifdef DEBUG
    if(m_id == 0) throw std::runtime_error("Trying to create an OpenGL index buffer with an id of 0.");
#endif
}

}