//
// Created by Killian on 17/04/2023.
//
#include "engine/renderers/opengl/VertexBuffer.h"

GLenum Engine::OpenGL::ShaderBaseTypeToOpenGLBaseType(ShaderBaseType type)
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
    }

    return 0;
}