//
// Created by Killian on 18/04/2023.
//
#pragma once

#include "engine/renderers/opengl/VertexBufferBase.h"
#include "engine/renderers/opengl/Shader.h"
#include "engine/renderers/Pipeline.h"
#include "engine/glad/glad.h"
#include "Texture.h"

namespace Engine::OpenGL
{

GLenum PrimitiveTopologyToOpenGLTopology(PrimitiveTopology topology);

extern uint32_t s_boundProgramId;

template <typename T>
class Pipeline : public Engine::Pipeline<T>
{
public:
    Pipeline(
            Vector2<uint32_t> viewportPos,
            Vector2<uint32_t> viewportSize,
            Vector2<uint32_t> scissorsPos,
            Vector2<uint32_t> scissorsSize,
            PrimitiveTopology topology) :
        m_viewportPos(viewportPos), m_viewportSize(viewportSize), m_scissorsPos(scissorsPos), m_scissorsSize(scissorsSize), m_topology(topology)
    {
        m_programId = glCreateProgram();
        glCreateVertexArrays(1, &m_vertexArrayId);

#ifdef DEBUG
        if(m_programId == 0)
            throw std::runtime_error("Failed to create OpenGL pipeline.");

        if(m_vertexArrayId == 0)
            throw std::runtime_error("Failed to create OpenGL vertex array.");
#endif
    }

    ~Pipeline() override
    {
        glDeleteVertexArrays(1, &m_vertexArrayId);
        glDeleteProgram(m_programId);

        if(m_programId == s_boundProgramId)
            s_boundProgramId = 0;
    }

    void Bind() override
    {
        if(m_programId != s_boundProgramId)
        {
            if(m_shouldLink)
            {
                if (m_vertexShader) glAttachShader(m_programId, m_vertexShader->GetId());
                if (m_tessellationControlShader) glAttachShader(m_programId, m_tessellationControlShader->GetId());
                if (m_tessellationEvaluationShader) glAttachShader(m_programId, m_tessellationEvaluationShader->GetId());
                if (m_geometryShader) glAttachShader(m_programId, m_geometryShader->GetId());
                if (m_fragmentShader) glAttachShader(m_programId, m_fragmentShader->GetId());
                if (m_computeShader) glAttachShader(m_programId, m_computeShader->GetId());

                glLinkProgram(m_programId);

                if (m_vertexShader) glDetachShader(m_programId, m_vertexShader->GetId());
                if (m_tessellationControlShader) glDetachShader(m_programId, m_tessellationControlShader->GetId());
                if (m_tessellationEvaluationShader) glDetachShader(m_programId, m_tessellationEvaluationShader->GetId());
                if (m_geometryShader) glDetachShader(m_programId, m_geometryShader->GetId());
                if (m_fragmentShader) glDetachShader(m_programId, m_fragmentShader->GetId());
                if (m_computeShader) glDetachShader(m_programId, m_computeShader->GetId());

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
            glViewport(m_viewportPos.x, m_viewportPos.y, m_viewportSize.x, m_viewportSize.y);
            glScissor(m_scissorsPos.x, m_scissorsPos.y, m_scissorsSize.x, m_scissorsSize.y);
            s_boundProgramId = m_programId;

            if(m_shouldUpdateBindings)
            {
                glBindVertexArray(m_vertexArrayId);
                this->m_vertexBuffer->Bind();
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
                VertexBufferLayout layout = T::GetLayout();
                for(auto& element : layout.GetElements())
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
                    struct { GLint type; GLint arraySize; GLint location; } results;
#pragma pack(pop)

                    glGetProgramResourceiv(
                        m_programId,
                        GL_UNIFORM,
                        i,
                        sizeof(properties) / sizeof(properties[0]),
                        properties,
                        sizeof(results) / sizeof(GLint),
                        nullptr,
                        (GLint*)&results
                    );

                    glGetProgramResourceName(m_programId, GL_UNIFORM, i, m_maxUniformNameSize, nullptr, m_uniformNameBuffer.data());

                    m_uniforms.emplace_back(
                        m_uniformNameBuffer.data(),
                        results.location,
                        OpenGLBaseTypeToShaderBaseType(results.type),
                        results.arraySize);
                }

                std::vector<VertexBufferLayout::Element> elements = layout.GetElements();

                for(int i = 0; i < m_inputCount; i++)
                {
                    static const GLenum properties[] = { GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION, };
#pragma pack(push, 1)
                    struct { GLint type; GLint arraySize; GLint location; } results;
#pragma pack(pop)

                    glGetProgramResourceiv(
                        m_programId,
                        GL_PROGRAM_INPUT,
                        i,
                        sizeof(properties) / sizeof(properties[0]),
                        properties,
                        sizeof(results) / sizeof(GLint),
                        nullptr,
                        (GLint*)&results
                    );

                    glGetProgramResourceName(m_programId, GL_PROGRAM_INPUT, i, m_maxInputNameSize, nullptr, m_inputNameBuffer.data());

                    bool found = false;
                    for(int j = 0; j < m_inputCount; j++)
                    {
                        if(std::strcmp(m_inputNames[j].data(), m_inputNameBuffer.data()) == 0)
                        {
                            auto& element = layout.GetElements()[j];

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
                                    static_cast<GLint>(layout.GetStride()),
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
    }

    void Unbind() override
    {
        if(s_boundProgramId != 0)
        {
            glUseProgram(0);
            s_boundProgramId = 0;
        }
    }

    void SetViewport(const Vector2<uint32_t>& pos, const Vector2<uint32_t>& size) override
    {
        m_viewportPos = pos;
        m_viewportSize = size;
    }

    void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override
    {
        m_scissorsPos = {x, y};
        m_scissorsSize = {width, height};
    }

    void SetVertexShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_Vertex)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_vertexShader = glShader;
        m_shouldLink = true;
    }

    void SetTessellationControlShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_TessellationControl)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_tessellationControlShader = glShader;
        m_shouldLink = true;
    }

    void SetTessellationEvaluationShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_TessellationEvaluation)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_tessellationEvaluationShader = glShader;
        m_shouldLink = true;
    }

    void SetGeometryShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_Geometry)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_geometryShader = glShader;
        m_shouldLink = true;
    }

    void SetFragmentShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_Fragment)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_fragmentShader = glShader;
        m_shouldLink = true;
    }

    void SetComputeShader(const std::shared_ptr<Engine::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ShaderType::ShaderType_Compute)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_computeShader = glShader;
        m_shouldLink = true;
    }

    void Render() override
    {
        Bind();

#ifdef DEBUG
        if(!m_bound)
            throw std::runtime_error("Program not bound.");

        if(!this->m_vertexBuffer)
            throw std::runtime_error("No vertex buffer set.");

        if(!this->m_indexBuffer)
            throw std::runtime_error("No index buffer set.");

        for(size_t i = 0; i < m_uniforms.size(); i++)
        {
            auto& uniform = m_uniforms[i];

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

        if(m_texture) m_texture->Bind();

        glBindVertexArray(m_vertexArrayId);

        glDrawElements(
                OpenGL::PrimitiveTopologyToOpenGLTopology(m_topology),
                static_cast<GLsizei>(this->m_indexBuffer->GetCount()),
                GL_UNSIGNED_INT,
                nullptr
                );
    }

    void SetUniform(const std::string& name, float value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Float)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a float." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform1f(uniform->location, value);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Vector2<float>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Float2)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a vec2." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform2f(uniform->location, value.x, value.y);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Vector3<float>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Float3)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a vec3." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform3f(uniform->location, value.x, value.y, value.z);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Vector4<float>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Float4)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a vec4." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform4f(uniform->location, value.x, value.y, value.z, value.w);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Matrix3<float>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Mat3)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a mat3." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniformMatrix3fv(uniform->location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value.data[0][0]));
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Matrix4<float>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Mat4)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a mat4." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniformMatrix4fv(uniform->location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value.data[0][0]));
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, int value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Int)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not an int." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform1i(uniform->location, value);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Vector2<int>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Int2)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a ivec2." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform2i(uniform->location, value.x, value.y);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const Vector3<int>& value) override
        {
            Bind();

            auto uniform = FindUniform(name);

            if(!uniform)
            {
                std::stringstream error;
                error << "Uniform " << name << " not found." << std::endl;
                error << "Available uniforms:" << std::endl;
                for(auto& _uniform : m_uniforms)
                {
                    error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
                }
                throw std::runtime_error(error.str());
            }

            if(uniform->type != ShaderBaseType::ShaderBaseType_Int3)
            {
                std::stringstream error;
                error << "Uniform " << name << " is not a ivec3." << std::endl;
                throw std::runtime_error(error.str());
            }

            glUniform3i(uniform->location, value.x, value.y, value.z);
            uniform->isSet = true;
        }

    void SetUniform(const std::string& name, const Vector4<int>& value) override
        {
            Bind();

            auto uniform = FindUniform(name);

            if(!uniform)
            {
                std::stringstream error;
                error << "Uniform " << name << " not found." << std::endl;
                error << "Available uniforms:" << std::endl;
                for(auto& _uniform : m_uniforms)
                {
                    error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
                }
                throw std::runtime_error(error.str());
            }

            if(uniform->type != ShaderBaseType::ShaderBaseType_Int4)
            {
                std::stringstream error;
                error << "Uniform " << name << " is not a ivec4." << std::endl;
                throw std::runtime_error(error.str());
            }

            glUniform4i(uniform->location, value.x, value.y, value.z, value.w);
            uniform->isSet = true;
        }

    void SetUniform(const std::string& name, bool value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Bool)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a bool." << std::endl;
            throw std::runtime_error(error.str());
        }

        glUniform1i(uniform->location, value);
        uniform->isSet = true;
    }

    void SetUniform(const std::string& name, const std::shared_ptr<Engine::Texture>& value) override
    {
        Bind();

        auto uniform = FindUniform(name);

        if(!uniform)
        {
            std::stringstream error;
            error << "Uniform " << name << " not found." << std::endl;
            error << "Available uniforms:" << std::endl;
            for(auto& _uniform : m_uniforms)
            {
                error << "  " << _uniform.name << "(" << ShaderBaseTypeToString(_uniform.type) << ")" << std::endl;
            }
            throw std::runtime_error(error.str());
        }

        if(uniform->type != ShaderBaseType::ShaderBaseType_Texture)
        {
            std::stringstream error;
            error << "Uniform " << name << " is not a texture." << std::endl;
            throw std::runtime_error(error.str());
        }

        m_texture = std::dynamic_pointer_cast<OpenGL::Texture>(value);
        uniform->isSet = true;
    }

protected:
    void SetVertexBuffer() override
    {
        m_shouldUpdateBindings = true;
    }

    void SetIndexBuffer() override
    {

    }

private:
    struct Uniform
    {
        std::string name;
        GLint location = -1;
        ShaderBaseType type = ShaderBaseType::ShaderBaseType_Unknown;
        uint32_t size = 0;
        bool isSet = false;
    };

    Uniform* FindUniform(const std::string& name)
    {
        auto it = std::find_if(
                m_uniforms.begin(),
                m_uniforms.end(),
                [&name](const Uniform& uniform) { return uniform.name == name; });

        if(it == m_uniforms.end())
        {
            return {};
        }

        return &*it;
    }

    uint32_t m_programId = 0;
    uint32_t m_vertexArrayId = 0;

    Vector2<uint32_t> m_viewportPos = {};
    Vector2<uint32_t> m_viewportSize = {};
    Vector2<uint32_t> m_scissorsPos = {};
    Vector2<uint32_t> m_scissorsSize = {};

    std::shared_ptr<OpenGL::Shader> m_vertexShader;
    std::shared_ptr<OpenGL::Shader> m_tessellationControlShader;
    std::shared_ptr<OpenGL::Shader> m_tessellationEvaluationShader;
    std::shared_ptr<OpenGL::Shader> m_geometryShader;
    std::shared_ptr<OpenGL::Shader> m_fragmentShader;
    std::shared_ptr<OpenGL::Shader> m_computeShader;

    bool m_shouldLink = false;
    bool m_shouldUpdateBindings = false;
    bool m_linked = false;
    bool m_bound = false;

    GLint m_uniformCount = 0;
    //GLint m_uniformBlockCount = 0;
    //GLint m_shaderStorageBlockCount = 0;
    GLint m_inputCount = 0;

    GLint m_maxUniformNameSize = 0;
    //GLint m_maxUniformBlockNameSize = 0;
    //GLint m_maxShaderStorageBlockNameSize = 0;
    GLint m_maxInputNameSize = 0;

    PrimitiveTopology m_topology = PrimitiveTopology::Triangles;

    std::vector<GLchar> m_uniformNameBuffer;
    //std::vector<GLchar> m_uniformBlockNameBuffer;
    //std::vector<GLchar> m_shaderStorageBlockNameBuffer;
    std::vector<GLchar> m_inputNameBuffer;

    std::vector<std::string> m_uniformNames;
    //std::vector<std::string> m_uniformBlockNames;
    //std::vector<std::string> m_shaderStorageBlockNames;
    std::vector<std::string> m_inputNames;

    std::vector<Uniform> m_uniforms;
    std::shared_ptr<OpenGL::Texture> m_texture;
    std::vector<GLint> m_boundAttributeLocations;
};

}