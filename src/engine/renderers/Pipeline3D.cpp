//
// Created by Killian on 20/04/2023.
//
#include "engine/renderers/Pipeline3D.h"
#include "engine/renderers/RendererObjects.h"

namespace Engine
{

Pipeline3D::Pipeline3D(const std::shared_ptr<Renderer>& renderer, std::shared_ptr<Pipeline> pipeline)
    : m_pipeline(std::move(pipeline))
{
    std::shared_ptr<Shader> vertexShader = CreateShader(renderer, ShaderType::ShaderType_Vertex);
    vertexShader->LoadFromFile("assets/shaders/Basic3D.vert.spv");
    m_pipeline->SetVertexShader(vertexShader);

    std::shared_ptr<Shader> fragmentShader = CreateShader(renderer, ShaderType::ShaderType_Fragment);
    fragmentShader->LoadFromFile("assets/shaders/Basic3D.frag.spv");
    m_pipeline->SetFragmentShader(fragmentShader);

    m_pipeline->LinkShaders();

    m_projectionMatrix = Matrix4<float>::Identity();
    m_viewMatrix = Matrix4<float>::Identity();
    m_modelMatrix = Matrix4<float>::Identity();

    m_pipeline->SetUniform("uniforms.MVP", m_projectionMatrix * m_viewMatrix * m_modelMatrix);
}

void Pipeline3D::SetProjectionMatrix(const Matrix4<float>& matrix)
{
    m_projectionMatrix = matrix;
    m_pipeline->SetUniform("uniforms.MVP", m_projectionMatrix * m_viewMatrix * m_modelMatrix);
}

void Pipeline3D::SetViewMatrix(const Matrix4<float>& matrix)
{
    m_viewMatrix = matrix;
    m_pipeline->SetUniform("uniforms.MVP", m_projectionMatrix * m_viewMatrix * m_modelMatrix);
}

void Pipeline3D::SetModelMatrix(const Matrix4<float>& matrix)
{
    m_modelMatrix = matrix;
    m_pipeline->SetUniform("uniforms.MVP", m_projectionMatrix * m_viewMatrix * m_modelMatrix);
}

}