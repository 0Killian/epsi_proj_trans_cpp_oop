//
// Created by Killian on 18/04/2023.
//
#pragma once

#include "engine/renderers/opengl/VertexBuffer.h"
#include "engine/renderers/opengl/Shader.h"
#include "engine/renderers/Pipeline.h"
#include "engine/glad/glad.h"

namespace OpenGL
{

GLenum PrimitiveTopologyToOpenGLTopology(PrimitiveTopology topology);

extern uint32_t s_boundProgramId;

template <typename T>
class Pipeline : public ::Pipeline<T>
{
public:
    Pipeline(
            ::Vector2<uint32_t> viewportPos,
            ::Vector2<uint32_t> viewportSize,
            ::Vector2<uint32_t> scissorsPos,
            ::Vector2<uint32_t> scissorsSize,
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
                else
                {
                    spdlog::info("Pipeline validation log: {}", std::string(infoLog.begin(), infoLog.end()));
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

                spdlog::info("Updating pipeline bindings.");
                glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &m_uniformCount);
                //glGetProgramInterfaceiv(m_programId, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &m_uniformBlockCount);
                //glGetProgramInterfaceiv(m_programId, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &m_shaderStorageBlockCount);
                glGetProgramInterfaceiv(m_programId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &m_inputCount);

                glGetProgramInterfaceiv(m_programId, GL_UNIFORM, GL_MAX_NAME_LENGTH, &m_maxUniformNameSize);
                //glGetProgramInterfaceiv(m_programId, GL_UNIFORM_BLOCK, GL_MAX_NAME_LENGTH, &m_maxUniformBlockNameSize);
                //glGetProgramInterfaceiv(m_programId, GL_SHADER_STORAGE_BLOCK, GL_MAX_NAME_LENGTH, &m_maxShaderStorageBlockNameSize);
                glGetProgramInterfaceiv(m_programId, GL_PROGRAM_INPUT, GL_MAX_NAME_LENGTH, &m_maxInputNameSize);

                m_uniformNameBuffer.clear();
                m_uniformNameBuffer.resize(m_maxUniformNameSize);

                m_inputNameBuffer.clear();
                m_inputNameBuffer.resize(m_maxInputNameSize);

                m_inputNames.clear();
                VertexBufferLayout layout = T::GetLayout();
                for(auto& element : layout.GetElements())
                {
                    m_inputNames.emplace_back(element.name);
                }

                if(m_uniformNames.size() != m_uniformCount)
                    throw std::runtime_error("Uniform count mismatch.");

                //if(m_uniformBlockNames.size() != m_uniformBlockCount)
                //    throw std::runtime_error("Uniform block count mismatch.");

                //if(m_shaderStorageBlockNames.size() != m_shaderStorageBlockCount)
                //    throw std::runtime_error("Shader storage block count mismatch.");

                if(m_inputNames.size() != m_inputCount)
                    throw std::runtime_error("Input count mismatch.");


                for(auto& i : m_boundAttributeLocations)
                {
                    glDisableVertexAttribArray(i);
                }


                const GLenum uniformProperties[] = {
                        GL_NAME_LENGTH,
                        GL_TYPE,
                        GL_ARRAY_SIZE,
                        GL_OFFSET,
                        GL_BLOCK_INDEX,
                        GL_ARRAY_STRIDE,
                        GL_MATRIX_STRIDE,
                        GL_IS_ROW_MAJOR,
                        GL_ATOMIC_COUNTER_BUFFER_INDEX,
                        GL_BUFFER_BINDING,
                        GL_BUFFER_DATA_SIZE,
                        GL_NUM_ACTIVE_VARIABLES,
                        GL_REFERENCED_BY_VERTEX_SHADER,
                        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
                        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
                        GL_REFERENCED_BY_GEOMETRY_SHADER,
                        GL_REFERENCED_BY_FRAGMENT_SHADER,
                        GL_REFERENCED_BY_COMPUTE_SHADER,
                        GL_LOCATION
                };

#pragma pack(push, 1)
                struct UniformResults
                {
                    GLint nameLength;
                    GLint type;
                    GLint arraySize;
                    GLint offset;
                    GLint blockIndex;
                    GLint arrayStride;
                    GLint matrixStride;
                    GLint isRowMajor;
                    GLint atomicCounterBufferIndex;
                    GLint bufferBinding;
                    GLint bufferDataSize;
                    GLint numActiveVariables;
                    GLboolean referencedByVertexShader;
                    GLboolean referencedByTessControlShader;
                    GLboolean referencedByTessEvaluationShader;
                    GLboolean referencedByGeometryShader;
                    GLboolean referencedByFragmentShader;
                    GLboolean referencedByComputeShader;
                    GLint location;
                } uniformResults;
#pragma pack(pop)

                for(int i = 0; i < m_uniformCount; i++)
                {
                    glGetProgramResourceiv(
                            m_programId,
                            GL_UNIFORM,
                            i,
                            sizeof(uniformProperties) / sizeof(uniformProperties[0]),
                            uniformProperties,
                            sizeof(UniformResults) / sizeof(GLint),
                            nullptr,
                            (GLint*)&uniformResults
                    );

                    glGetProgramResourceName(m_programId, GL_UNIFORM, i, m_maxUniformNameSize, nullptr, m_uniformNameBuffer.data());

                    // Debug
                    spdlog::debug("Uniform {}: {}", i, m_uniformNameBuffer.data());
                    spdlog::debug("  Name length: {}", uniformResults.nameLength);
                    spdlog::debug("  Type: {}", uniformResults.type);
                    spdlog::debug("  Array size: {}", uniformResults.arraySize);
                    spdlog::debug("  Offset: {}", uniformResults.offset);
                    spdlog::debug("  Block index: {}", uniformResults.blockIndex);
                    spdlog::debug("  Array stride: {}", uniformResults.arrayStride);
                    spdlog::debug("  Matrix stride: {}", uniformResults.matrixStride);
                    spdlog::debug("  Is row major: {}", uniformResults.isRowMajor);
                    spdlog::debug("  Atomic counter buffer index: {}", uniformResults.atomicCounterBufferIndex);
                    spdlog::debug("  Buffer binding: {}", uniformResults.bufferBinding);
                    spdlog::debug("  Buffer data size: {}", uniformResults.bufferDataSize);
                    spdlog::debug("  Num active variables: {}", uniformResults.numActiveVariables);
                    spdlog::debug("  Referenced by vertex shader: {}", uniformResults.referencedByVertexShader);
                    spdlog::debug("  Referenced by tess control shader: {}", uniformResults.referencedByTessControlShader);
                    spdlog::debug("  Referenced by tess evaluation shader: {}", uniformResults.referencedByTessEvaluationShader);
                    spdlog::debug("  Referenced by geometry shader: {}", uniformResults.referencedByGeometryShader);
                    spdlog::debug("  Referenced by fragment shader: {}", uniformResults.referencedByFragmentShader);
                    spdlog::debug("  Referenced by compute shader: {}", uniformResults.referencedByComputeShader);
                    spdlog::debug("  Location: {}", uniformResults.location);
                }

                const GLenum inputProperties[] = {
                        GL_NAME_LENGTH,
                        GL_TYPE,
                        GL_ARRAY_SIZE,
                        GL_LOCATION,
                        GL_REFERENCED_BY_VERTEX_SHADER,
                        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
                        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
                        GL_REFERENCED_BY_GEOMETRY_SHADER,
                        GL_REFERENCED_BY_FRAGMENT_SHADER,
                        GL_REFERENCED_BY_COMPUTE_SHADER
                };

#pragma pack(push, 1)
                struct InputResults
                {
                    GLint nameLength;
                    GLint type;
                    GLint arraySize;
                    GLint location;
                    GLboolean referencedByVertexShader;
                    GLboolean referencedByTessControlShader;
                    GLboolean referencedByTessEvaluationShader;
                    GLboolean referencedByGeometryShader;
                    GLboolean referencedByFragmentShader;
                    GLboolean referencedByComputeShader;
                } inputResults;
#pragma pack(pop)

                for(int i = 0; i < m_inputCount; i++)
                {
                    glGetProgramResourceiv(
                            m_programId,
                            GL_PROGRAM_INPUT,
                            i,
                            sizeof(inputProperties) / sizeof(inputProperties[0]),
                            inputProperties,
                            sizeof(InputResults) / sizeof(GLint),
                            nullptr,
                            (GLint*)&inputResults
                    );

                    glGetProgramResourceName(m_programId, GL_PROGRAM_INPUT, i, m_maxInputNameSize, nullptr, m_inputNameBuffer.data());

                    // Debug
                    spdlog::debug("Input {}: {}", i, m_inputNameBuffer.data());
                    spdlog::debug("  Name length: {}", inputResults.nameLength);
                    spdlog::debug("  Type: {}", inputResults.type);
                    spdlog::debug("  Array size: {}", inputResults.arraySize);
                    spdlog::debug("  Location: {}", inputResults.location);
                    spdlog::debug("  Referenced by vertex shader: {}", inputResults.referencedByVertexShader);
                    spdlog::debug("  Referenced by tess control shader: {}", inputResults.referencedByTessControlShader);
                    spdlog::debug("  Referenced by tess evaluation shader: {}", inputResults.referencedByTessEvaluationShader);
                    spdlog::debug("  Referenced by geometry shader: {}", inputResults.referencedByGeometryShader);
                    spdlog::debug("  Referenced by fragment shader: {}", inputResults.referencedByFragmentShader);
                    spdlog::debug("  Referenced by compute shader: {}", inputResults.referencedByComputeShader);

                    bool found = false;
                    for(int j = 0; j < m_inputCount; j++)
                    {
                        if(std::strcmp(m_inputNames[j].data(), m_inputNameBuffer.data()) == 0)
                        {
                            // TODO: check if element layout is correct
                            auto& element = layout.GetElements()[j];
                            glEnableVertexAttribArray(inputResults.location);
                            glVertexAttribPointer(
                                    inputResults.location,
                                    static_cast<GLint>(element.componentCount),
                                    OpenGL::ShaderDataTypeToOpenGLBaseType(element.type),
                                    element.normalized ? GL_TRUE : GL_FALSE,
                                    static_cast<GLint>(layout.GetStride()),
                                    reinterpret_cast<void*>(element.offset)
                                    );

                            found = true;
                            break;
                        }
                    }

                    if(!found)
                    {
                        throw std::runtime_error("Incorrect vertex buffer layout");
                    }
                }

                m_shouldUpdateBindings = false;
                m_bound = true;

                glBindVertexArray(0);
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

    void SetViewport(const ::Vector2<uint32_t>& pos, const ::Vector2<uint32_t>& size) override
    {
        m_viewportPos = pos;
        m_viewportSize = size;
    }

    void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override
    {
        m_scissorsPos = {x, y};
        m_scissorsSize = {width, height};
    }

    void SetVertexShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::Vertex)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_vertexShader = glShader;
        m_shouldLink = true;
    }

    void SetTessellationControlShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::TessellationControl)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_tessellationControlShader = glShader;
        m_shouldLink = true;
    }

    void SetTessellationEvaluationShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::TessellationEvaluation)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_tessellationEvaluationShader = glShader;
        m_shouldLink = true;
    }

    void SetGeometryShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::Geometry)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_geometryShader = glShader;
        m_shouldLink = true;
    }

    void SetFragmentShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::Fragment)
            throw std::runtime_error("Invalid shader type.");
#endif

        m_fragmentShader = glShader;
        m_shouldLink = true;
    }

    void SetComputeShader(const std::shared_ptr<::Shader>& shader) override
    {
        const std::shared_ptr<OpenGL::Shader>& glShader = std::dynamic_pointer_cast<OpenGL::Shader>(shader);

#ifdef DEBUG
        if(glShader && glShader->GetType() != ::Shader::Type::Compute)
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
#endif

        glBindVertexArray(m_vertexArrayId);

        glDrawElements(
                OpenGL::PrimitiveTopologyToOpenGLTopology(m_topology),
                static_cast<GLsizei>(this->m_indexBuffer->GetCount()),
                GL_UNSIGNED_INT,
                nullptr
                );

        glBindVertexArray(0);
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
    uint32_t m_programId = 0;
    uint32_t m_vertexArrayId = 0;

    ::Vector2<uint32_t> m_viewportPos = {};
    ::Vector2<uint32_t> m_viewportSize = {};
    ::Vector2<uint32_t> m_scissorsPos = {};
    ::Vector2<uint32_t> m_scissorsSize = {};

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

    std::vector<GLint> m_boundAttributeLocations;
};

}