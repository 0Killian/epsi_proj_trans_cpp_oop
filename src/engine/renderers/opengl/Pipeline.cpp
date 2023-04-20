//
// Created by Killian on 18/04/2023.
//
#include "engine/renderers/opengl/Pipeline.h"

namespace Engine::OpenGL
{

GLenum PrimitiveTopologyToOpenGLTopology(PrimitiveTopology topology)
{
    switch(topology)
    {
    case PrimitiveTopology::Points:return GL_POINTS;
    case PrimitiveTopology::Lines:return GL_LINES;
    case PrimitiveTopology::LineStrip:return GL_LINE_STRIP;
    case PrimitiveTopology::TriangleStrip:return GL_TRIANGLE_STRIP;
    case PrimitiveTopology::TriangleFan:return GL_TRIANGLE_FAN;
    default:return GL_TRIANGLES;
    }
}

Pipeline::Pipeline(
    Vector2<uint32_t> viewportPos,
    Vector2<uint32_t> viewportSize,
    Vector2<uint32_t> scissorsPos,
    Vector2<uint32_t> scissorsSize,
    PrimitiveTopology topology)
    : m_viewportPos(viewportPos),
      m_viewportSize(viewportSize),
      m_scissorsPos(scissorsPos),
      m_scissorsSize(scissorsSize),
      m_topology(topology)
{
    m_programId = glCreateProgram();
    glCreateVertexArrays(1, &m_vertexArrayId);

#ifdef DEBUG
    if(m_programId == 0) throw std::runtime_error("Failed to create OpenGL pipeline.");
    if(m_vertexArrayId == 0) throw std::runtime_error("Failed to create OpenGL vertex array.");
#endif
}

Pipeline::~Pipeline()
{
    glDeleteVertexArrays(1, &m_vertexArrayId);
    glDeleteProgram(m_programId);
}

void Pipeline::Bind()
{
    if(m_shouldLink)
    {
        if (m_vertexShader) glAttachShader(m_programId, m_vertexShader->m_id);
        if (m_tessellationControlShader) glAttachShader(m_programId, m_tessellationControlShader->m_id);
        if (m_tessellationEvaluationShader) glAttachShader(m_programId, m_tessellationEvaluationShader->m_id);
        if (m_geometryShader) glAttachShader(m_programId, m_geometryShader->m_id);
        if (m_fragmentShader) glAttachShader(m_programId, m_fragmentShader->m_id);
        if (m_computeShader) glAttachShader(m_programId, m_computeShader->m_id);

        glLinkProgram(m_programId);

        if (m_vertexShader) glDetachShader(m_programId, m_vertexShader->m_id);
        if (m_tessellationControlShader) glDetachShader(m_programId, m_tessellationControlShader->m_id);
        if (m_tessellationEvaluationShader) glDetachShader(m_programId, m_tessellationEvaluationShader->m_id);
        if (m_geometryShader) glDetachShader(m_programId, m_geometryShader->m_id);
        if (m_fragmentShader) glDetachShader(m_programId, m_fragmentShader->m_id);
        if (m_computeShader) glDetachShader(m_programId, m_computeShader->m_id);

        GLint isLinked = 0;
        glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_programId, maxLength, &maxLength, infoLog.data());

            throw std::runtime_error(std::string(infoLog.begin(), infoLog.end()));
        }

#ifdef DEBUG
        glValidateProgram(m_programId);

        GLint maxLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, infoLog.data());

        GLint isValid = 0;
        glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &isValid);
        if (isValid == GL_FALSE)
        {
            throw std::runtime_error(std::string(infoLog.begin(), infoLog.end()));
        }
#endif

        m_shouldLink = false;
        if(m_linked) m_shouldUpdateBindings = true;
        m_linked = true;
    }

    glUseProgram(m_programId);
    glBindVertexArray(m_vertexArrayId);
    glViewport(m_viewportPos.x, m_viewportPos.y, m_viewportSize.x, m_viewportSize.y);
    glScissor(m_scissorsPos.x, m_scissorsPos.y, m_scissorsSize.x, m_scissorsSize.y);


    if(m_shouldUpdateBindings)
    {
        glBindVertexArray(m_vertexArrayId);
        this->m_vertexBufferBase->Bind();
        this->m_indexBuffer->Bind();

        glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &m_uniformCount);
        glGetProgramInterfaceiv(m_programId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &m_inputCount);

        glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_MAX_NAME_LENGTH, &m_maxUniformNameSize);
        glGetProgramInterfaceiv(m_programId, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &m_maxInputNameSize);

        m_uniformNameBuffer.clear();
        m_uniformNameBuffer.resize(m_maxUniformNameSize);

        m_inputNameBuffer.clear();
        m_inputNameBuffer.resize(m_maxInputNameSize);

        m_inputNames.clear();
        for(auto& element : m_layout.GetElements())
        {
            if(element.type == ShaderBaseType::ShaderBaseType_Texture)
                throw std::runtime_error("Texture type is not supported in vertex buffer layout.");

            m_inputNames.emplace_back(element.name);
        }

        for(auto& i : m_boundAttributeLocations)
        {
            glDisableVertexAttribArray(i);
        }

        m_boundAttributeLocations.clear();

        m_uniforms.clear();

        for(int i = 0; i < m_uniformCount; i++)
        {
            static const GLenum properties[] = { GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION };
#pragma pack(push, 1)
            struct { GLint type; GLint arraySize; GLint location; } results = {};
#pragma pack(pop)

            glGetProgramResourceiv(
                m_programId,
                GL_UNIFORM,
                i,
                sizeof(properties) / sizeof(properties[0]),
                properties,
                sizeof(results) / sizeof(GLint),
                nullptr,
                reinterpret_cast<GLint*>(&results)
            );

            glGetProgramResourceName(
                m_programId,
                GL_UNIFORM,
                i,
                m_maxUniformNameSize,
                nullptr,
                m_uniformNameBuffer.data()
            );

            m_uniforms.emplace_back(
                m_uniformNameBuffer.data(),
                results.location,
                OpenGLBaseTypeToShaderBaseType(results.type),
                results.arraySize
            );
        }

        std::vector<VertexBufferLayout::Element> elements = m_layout.GetElements();

        for(int i = 0; i < m_inputCount; i++)
        {
            static const GLenum properties[] = { GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION, };
#pragma pack(push, 1)
            struct { GLint type; GLint arraySize; GLint location; } results = {};
#pragma pack(pop)

            glGetProgramResourceiv(
                m_programId,
                GL_PROGRAM_INPUT,
                i,
                sizeof(properties) / sizeof(properties[0]),
                properties,
                sizeof(results) / sizeof(GLint),
                nullptr,
                reinterpret_cast<GLint*>(&results)
            );

            glGetProgramResourceName(
                m_programId,
                GL_PROGRAM_INPUT,
                i,
                m_maxInputNameSize,
                nullptr,
                m_inputNameBuffer.data()
            );

            bool found = false;
            for(int j = 0; j < m_inputCount; j++)
            {
                if(std::strcmp(m_inputNames[j].data(), m_inputNameBuffer.data()) == 0)
                {
                    auto& element = m_layout.GetElements()[j];

                    if(element.type != OpenGLBaseTypeToShaderBaseType(results.type))
                    {
                        std::stringstream error;
                        error << "Incorrect buffer layout: " << std::endl;
                        error << "  Input " << m_inputNameBuffer.data() << " (at location " << results.location << ") has incorrect type" << std::endl;
                        error << "  Expected " << ShaderBaseTypeToString(OpenGLBaseTypeToShaderBaseType(results.type))
                              << " but got " << ShaderBaseTypeToString(element.type) << std::endl;
                        throw std::runtime_error(error.str());
                    }

                    glEnableVertexAttribArray(results.location);
                    // TODO: https://www.khronos.org/opengl/wiki/Vertex_Specification#Separate_attribute_format
                    glVertexAttribPointer(
                        results.location,
                        static_cast<GLint>(element.componentCount),
                        OpenGL::ShaderBaseTypeToOpenGLBaseType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        static_cast<GLint>(m_layout.GetStride()),
                        reinterpret_cast<void*>(element.offset)
                    );

                    m_boundAttributeLocations.push_back(results.location);
                    elements.erase(std::find(elements.begin(), elements.end(), element));

                    found = true;
                    break;
                }
            }

            if(!found)
            {
                std::stringstream error;
                error << "Incorrect buffer layout: " << std::endl;
                error << "  Input " << m_inputNameBuffer.data() << " (at location " << results.location << ") not found in layout" << std::endl;
                error << "  This input is of type " << OpenGL::ShaderBaseTypeToString(OpenGL::OpenGLBaseTypeToShaderBaseType(results.type)) << " and has " << results.arraySize << " elements" << std::endl;
                error << "Please check that the shader input declarations are correct, and they includes all the elements in the layout" << std::endl;
                throw std::runtime_error(error.str());
            }
        }

        if(!elements.empty())
        {
            std::stringstream error;
            error << "Incorrect buffer layout: " << std::endl;
            error << "The following elements are not bound to any input:" << std::endl;
            for(auto& element : elements)
            {
                error << "  " << element.name << " (" << OpenGL::ShaderBaseTypeToString(element.type) << ")" << std::endl;
            }

            error << "Please check that the shader input declarations are correct, and they includes all the elements in the layout" << std::endl;
            throw std::runtime_error(error.str());
        }

        m_shouldUpdateBindings = false;
        m_bound = true;
    }

    if(!m_bound)
    {
        throw std::runtime_error("Failed to bind program");
    }
}

void Pipeline::Unbind()
{
    glUseProgram(0);
}

void Pipeline::SetViewport(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size)
{
    m_viewportPos = pos;
    m_viewportSize = size;
}

void Pipeline::SetScissor(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size)
{
    m_scissorsPos = pos;
    m_scissorsSize = size;
}

#ifdef DEBUG
#define SET_SHADER(shaderType, shaderName)                                                                      \
void Pipeline::Set##shaderType##Shader(const std::shared_ptr<Engine::Shader>& shader)                           \
{                                                                                                               \
    const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);        \
    if(!glShader) throw std::runtime_error("Shader was null.");                                                 \
    if(glShader->m_type != ShaderType::ShaderType_##shaderType) throw std::runtime_error("Invalid shader type.");     \
    m_##shaderName##Shader = glShader;                                                                          \
    m_shouldLink = true;                                                                                        \
}
#else
#define SET_SHADER(shaderType, shaderName) \
void Pipeline::Set##shaderType##Shader(const std::shared_ptr<Engine::Shader>& shader) \
{ \
    m_##shaderName##Shader = std::dynamic_pointer_cast<OpenGL::Shader>(shader); \
    m_shouldLink = true; \
}
#endif

SET_SHADER(Vertex, vertex)
SET_SHADER(Fragment, fragment)
SET_SHADER(Geometry, geometry)
SET_SHADER(TessellationControl, tessellationControl)
SET_SHADER(TessellationEvaluation, tessellationEvaluation)
SET_SHADER(Compute, compute)

void Pipeline::Render()
{
    Bind();

#ifdef DEBUG
    if(!m_bound) throw std::runtime_error("Program not bound.");
    if(!m_vertexBufferBase) throw std::runtime_error("No vertex buffer set.");
    if(!m_indexBuffer) throw std::runtime_error("No index buffer set.");

    for(auto & uniform : m_uniforms)
    {
        if(!uniform.isSet)
        {
            std::stringstream error;
            if(uniform.type == ShaderBaseType::ShaderBaseType_Texture)
                error << "A texture needs to be set to this pipeline." << std::endl;
            else
                error << "Uniform " << uniform.name << " is not set." << std::endl;
            throw std::runtime_error(error.str());
        }
    }
#endif

    if(m_texture)
    {
        // TODO: Support more than one texture
        glActiveTexture(GL_TEXTURE0);
        m_texture->Bind();
    }

    glDrawElements(
            OpenGL::PrimitiveTopologyToOpenGLTopology(m_topology),
            static_cast<GLsizei>(this->m_indexBuffer->GetCount()),
            GL_UNSIGNED_INT,
            nullptr
    );
}

#define SET_UNIFORM(functionType, shaderBaseTypeName, typeName, setMethod)                                              \
void Pipeline::SetUniform(const std::string& name, functionType value)                                                  \
{                                                                                                               \
    Bind();                                                                                                     \
    auto uniform = FindUniform(name);                                                                           \
    if(!uniform)                                                                                                \
    {                                                                                                           \
        std::stringstream error;                                                                                \
        error << "Uniform " << name << " not found." << std::endl;                                              \
        error << "Available uniforms:" << std::endl;                                                            \
        for(auto& _uniform : m_uniforms)                                                                        \
        {                                                                                                       \
            error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl; \
        }                                                                                                       \
        throw std::runtime_error(error.str());                                                                  \
    }                                                                                                           \
    if(uniform->type != ShaderBaseType::ShaderBaseType_##shaderBaseTypeName)                                    \
    {                                                                                                           \
        std::stringstream error;                                                                                \
        error << "Uniform " << name << " is not a " #typeName "." << std::endl;                                 \
        throw std::runtime_error(error.str());                                                                  \
    }                                                                                                           \
    setMethod;                                                                                                  \
    uniform->isSet = true;                                                                                      \
}

SET_UNIFORM(float, Float, float, glUniform1f(uniform->location, value))
SET_UNIFORM(const Vector2<float>&, Float2, vec2, glUniform2f(uniform->location, value.x, value.y))
SET_UNIFORM(const Vector3<float>&, Float3, vec3, glUniform3f(uniform->location, value.x, value.y, value.z))
SET_UNIFORM(const Vector4<float>&, Float4, vec4, glUniform4f(uniform->location, value.x, value.y, value.z, value.w))
SET_UNIFORM(const Matrix3<float>&, Mat3, mat3,
            glUniformMatrix3fv(uniform->location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value.data[0][0])))
SET_UNIFORM(const Matrix4<float>&, Mat4, mat4,
            glUniformMatrix4fv(uniform->location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value.data[0][0])))
SET_UNIFORM(int, Int, int, glUniform1i(uniform->location, value))
SET_UNIFORM(const Vector2<int>&, Int2, ivec2, glUniform2i(uniform->location, value.x, value.y))
SET_UNIFORM(const Vector3<int>&, Int3, ivec3, glUniform3i(uniform->location, value.x, value.y, value.z))
SET_UNIFORM(const Vector4<int>&, Int4, ivec4, glUniform4i(uniform->location, value.x, value.y, value.z, value.w))
SET_UNIFORM(bool, Bool, bool, glUniform1i(uniform->location, value))
SET_UNIFORM(const std::shared_ptr<Engine::Texture>&, Texture, texture,
            m_texture = std::dynamic_pointer_cast<OpenGL::Texture>(value);)

void Pipeline::SetVertexBuffer(
        const std::shared_ptr<Engine::VertexBufferBase> &vertexBufferBase,
        const Engine::VertexBufferLayout &layout)
{
    m_vertexBufferBase = std::dynamic_pointer_cast<OpenGL::VertexBufferBase>(vertexBufferBase);
    m_layout = layout;
    m_shouldUpdateBindings = true;
}

void Pipeline::SetIndexBuffer(const std::shared_ptr<Engine::IndexBuffer> &indexBuffer)
{
    m_indexBuffer = std::dynamic_pointer_cast<OpenGL::IndexBuffer>(indexBuffer);
    m_shouldUpdateBindings = true;
}

Pipeline::Uniform* Pipeline::FindUniform(const std::string& name)
{
    auto it = std::find_if(
        m_uniforms.begin(),
        m_uniforms.end(),
        [&name](const Uniform& uniform) { return uniform.name == name; }
    );

    if(it == m_uniforms.end())
    {
        return {};
    }

    return &*it;
}

}