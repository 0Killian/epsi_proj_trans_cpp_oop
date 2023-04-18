//
// Created by Killian on 17/04/2023.
//
#include "engine/renderers/opengl/VertexBuffer.h"

GLenum OpenGL::ShaderDataTypeToOpenGLBaseType(VertexBufferLayout::Type type)
{
    switch (type)
    {
    case VertexBufferLayout::Type::Float:
    case VertexBufferLayout::Type::Float2:
    case VertexBufferLayout::Type::Float3:
    case VertexBufferLayout::Type::Float4:
    case VertexBufferLayout::Type::Mat3:
    case VertexBufferLayout::Type::Mat4:
        return GL_FLOAT;
    case VertexBufferLayout::Type::Int:
    case VertexBufferLayout::Type::Int2:
    case VertexBufferLayout::Type::Int3:
    case VertexBufferLayout::Type::Int4:
        return GL_INT;
    case VertexBufferLayout::Type::Bool:
        return GL_BOOL;
    }

    return 0;
}