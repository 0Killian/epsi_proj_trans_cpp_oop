//
// Created by Killian on 17/04/2023.
//
#include "engine/renderers/opengl/VertexBufferBase.h"

namespace Engine::OpenGL
{

GLenum ShaderBaseTypeToOpenGLBaseType(ShaderBaseType type)
{
    switch (type)
    {
    case ShaderBaseType::ShaderBaseType_Float:
    case ShaderBaseType::ShaderBaseType_Float2:
    case ShaderBaseType::ShaderBaseType_Float3:
    case ShaderBaseType::ShaderBaseType_Float4:
    case ShaderBaseType::ShaderBaseType_Mat3:
    case ShaderBaseType::ShaderBaseType_Mat4:
        return GL_FLOAT;
    case ShaderBaseType::ShaderBaseType_Int:
    case ShaderBaseType::ShaderBaseType_Int2:
    case ShaderBaseType::ShaderBaseType_Int3:
    case ShaderBaseType::ShaderBaseType_Int4:
        return GL_INT;
    case ShaderBaseType::ShaderBaseType_Bool:
        return GL_BOOL;
    default:break;
    }

    return 0;
}

ShaderBaseType OpenGLBaseTypeToShaderBaseType(GLenum type)
{
    switch(type)
    {
    case GL_FLOAT:return ShaderBaseType::ShaderBaseType_Float;
    case GL_FLOAT_VEC2:return ShaderBaseType::ShaderBaseType_Float2;
    case GL_FLOAT_VEC3:return ShaderBaseType::ShaderBaseType_Float3;
    case GL_FLOAT_VEC4:return ShaderBaseType::ShaderBaseType_Float4;
    case GL_FLOAT_MAT3:return ShaderBaseType::ShaderBaseType_Mat3;
    case GL_FLOAT_MAT4:return ShaderBaseType::ShaderBaseType_Mat4;
    case GL_INT:return ShaderBaseType::ShaderBaseType_Int;
    case GL_INT_VEC2:return ShaderBaseType::ShaderBaseType_Int2;
    case GL_INT_VEC3:return ShaderBaseType::ShaderBaseType_Int3;
    case GL_INT_VEC4:return ShaderBaseType::ShaderBaseType_Int4;
    case GL_BOOL:return ShaderBaseType::ShaderBaseType_Bool;
    case GL_SAMPLER_2D:return ShaderBaseType::ShaderBaseType_Texture;
    default:break;
    }
    return ShaderBaseType::ShaderBaseType_Unknown;
}

std::string ShaderBaseTypeToString(ShaderBaseType type)
{
    switch (type)
    {
    case ShaderBaseType::ShaderBaseType_Float:return "float";
    case ShaderBaseType::ShaderBaseType_Float2:return "vec2";
    case ShaderBaseType::ShaderBaseType_Float3:return "vec3";
    case ShaderBaseType::ShaderBaseType_Float4:return "vec4";
    case ShaderBaseType::ShaderBaseType_Mat3:return "mat3";
    case ShaderBaseType::ShaderBaseType_Mat4:return "mat4";
    case ShaderBaseType::ShaderBaseType_Int:return "int";
    case ShaderBaseType::ShaderBaseType_Int2:return "ivec2";
    case ShaderBaseType::ShaderBaseType_Int3:return "ivec3";
    case ShaderBaseType::ShaderBaseType_Int4:return "ivec4";
    case ShaderBaseType::ShaderBaseType_Bool:return "bool";
    case ShaderBaseType::ShaderBaseType_Texture:return "sampler2D";
    case ShaderBaseType::ShaderBaseType_Unknown:break;
    }
    return "unknown";
}

VertexBufferBase::VertexBufferBase(VertexBufferUsage usage) : m_usage(usage)
{
    glGenBuffers(1, &m_id);

#ifdef DEBUG
    if(m_id == 0) throw std::runtime_error("Failed to create OpenGL VertexBuffer");
#endif
};

std::vector<std::shared_ptr<Engine::VertexBufferBase>> VertexBufferBase::CreateVertexBuffers(VertexBufferUsage usage, int count)
{
    // TODO: profile multiple vertex buffer creation vs one by one
    std::vector<std::shared_ptr<Engine::VertexBufferBase>> buffers(count);
    std::vector<uint32_t> ids(count);
    glGenBuffers(count, ids.data());
    std::transform(ids.begin(), ids.end(), buffers.begin(), [&](uint32_t id)
    { return std::shared_ptr<Engine::VertexBufferBase>(new VertexBufferBase(usage, id)); });

    return buffers;
}

void VertexBufferBase::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBufferBase::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferBase::SetData(const void* data, size_t size)
{
    Bind();

    switch(m_usage)
    {
    case VertexBufferUsage::VertexBufferUsage_Static:
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
        break;
    case VertexBufferUsage::VertexBufferUsage_Dynamic:
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_DYNAMIC_DRAW);
        break;
    case VertexBufferUsage::VertexBufferUsage_Stream:
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STREAM_DRAW);
        break;
    }
}

void VertexBufferBase::UpdateData(const void* data, size_t size, size_t offset)
{
    if(size == 0) return;

    Bind();

#ifdef DEBUG
    GLint bufferSize;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

    if(offset + size > bufferSize) throw std::runtime_error("Trying to update data outside of the buffer");
    if(m_mapped) throw std::runtime_error("Trying to update data while the buffer is mapped");
    if(data == nullptr) throw std::runtime_error("Trying to update data with a null pointer");
#endif

    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
}

void* VertexBufferBase::Map(size_t offset, size_t size)
{
    Bind();

    GLint bufferSize;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

    if(size == 0)
    {
        size = bufferSize;
    }

#ifdef DEBUG
    if(offset + size > bufferSize) throw std::runtime_error("Trying to update data outside of the buffer");
    if(m_mapped) throw std::runtime_error("Trying to map data while the buffer is mapped");
#endif

    void* data = glMapBufferRange(
            GL_ARRAY_BUFFER,
            static_cast<GLintptr>(offset),
            static_cast<GLint>(size),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT
    );

    m_mapped = true;
    return data;
}

VertexBufferBase::~VertexBufferBase()
{
#ifdef DEBUG
    if(m_id == 0) spdlog::error("Trying to delete an already deleted OpenGL VertexBuffer");
    if(m_mapped) spdlog::error("Trying to delete an OpenGL VertexBuffer that is still mapped");
#endif

    glDeleteBuffers(1, &m_id);
}

void VertexBufferBase::Unmap()
{
    Bind();
    glUnmapBuffer(GL_ARRAY_BUFFER);
    m_mapped = false;
}

VertexBufferBase::VertexBufferBase(VertexBufferUsage usage, uint32_t id)
    : m_id(id), m_usage(usage)
{}

}