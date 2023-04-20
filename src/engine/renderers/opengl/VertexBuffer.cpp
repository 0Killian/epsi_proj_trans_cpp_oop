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
    default:break;
    }

    return 0;
}

Engine::ShaderBaseType Engine::OpenGL::OpenGLBaseTypeToShaderBaseType(GLenum type)
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

std::string Engine::OpenGL::ShaderBaseTypeToString(ShaderBaseType type)
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