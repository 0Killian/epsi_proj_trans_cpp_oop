//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/opengl/VertexBufferBase.h"
#include "engine/renderers/opengl/IndexBuffer.h"
#include "engine/renderers/opengl/Shader.h"
#include "engine/renderers/opengl/Pipeline.h"
#include "engine/renderers/opengl/Texture.h"

//#include "renderers/vulkan/VertexBuffer.h"
//#include "renderers/directx/VertexBuffer.h"
//#include "renderers/metal/VertexBuffer.h"

namespace Engine
{
template<typename T>
std::shared_ptr<VertexBuffer<T>> CreateVertexBuffer(const std::shared_ptr<Renderer>& renderer, VertexBufferUsage usage)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<VertexBuffer<T>>(std::make_shared<OpenGL::VertexBufferBase>(usage));
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::VertexBuffer<T>>(usage);
        throw std::runtime_error("Vulkan::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::VertexBuffer<T>>(usage);
        throw std::runtime_error("DirectX::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::VertexBuffer<T>>(usage);
        throw std::runtime_error("Metal::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    default:return nullptr;
    }
}

template<typename T>
std::vector<std::shared_ptr<VertexBuffer<T>>>
CreateVertexBuffers(const std::shared_ptr<Renderer>& renderer, VertexBufferUsage usage, size_t count)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
    {
        auto bufferBases = OpenGL::VertexBufferBase::CreateVertexBuffers(usage, count);
        std::vector<std::shared_ptr<VertexBuffer<T>>> buffers;
        buffers.reserve(bufferBases.size());
        for (auto &bufferBase: bufferBases)
        {
            buffers.emplace_back(std::make_shared<VertexBuffer<T>>(bufferBase));
        }
    }
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::VertexBuffer<T>>(usage);
        throw std::runtime_error("Vulkan::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::VertexBuffer<T>>(usage);
        throw std::runtime_error("DirectX::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::VertexBuffer<T>>(usage);
        throw std::runtime_error("Metal::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    default:return {};
    }
}

std::shared_ptr<IndexBuffer> CreateIndexBuffer(const std::shared_ptr<Renderer>& renderer, IndexBufferUsage usage)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:return std::make_shared<OpenGL::IndexBuffer>(usage);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::IndexBuffer>(usage);
        throw std::runtime_error("Vulkan::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::IndexBuffer>(usage);
        throw std::runtime_error("DirectX::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::IndexBuffer>(usage);
        throw std::runtime_error("Metal::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    default:return nullptr;
    }
}

std::vector<std::shared_ptr<IndexBuffer>>
CreateIndexBuffers(const std::shared_ptr<Renderer>& renderer, IndexBufferUsage usage, size_t count)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:return OpenGL::IndexBuffer::CreateIndexBuffers(usage, count);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::IndexBuffer>(usage);
        throw std::runtime_error("Vulkan::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::IndexBuffer>(usage);
        throw std::runtime_error("DirectX::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::IndexBuffer>(usage);
        throw std::runtime_error("Metal::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    default:return {};
    }
}

std::shared_ptr<Shader> CreateShader(const std::shared_ptr<Renderer>& renderer, ShaderType type)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:return std::make_shared<OpenGL::Shader>(type);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("Vulkan::Shader::CreateShader() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("DirectX::Shader::CreateShader() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("Metal::Shader::CreateShader() is not implemented yet.");
    default:return nullptr;
    }
}

template<typename T>
std::shared_ptr<Pipeline<T>>
CreatePipeline(
        const std::shared_ptr<Renderer>& renderer,
        Vector2<uint32_t> viewportPos,
        Vector2<uint32_t> viewportSize,
        Vector2<uint32_t> scissorsPos,
        Vector2<uint32_t> scissorsSize,
        PrimitiveTopology primitiveTopology)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<OpenGL::Pipeline<T>>(viewportPos, viewportSize, scissorsPos, scissorsSize,
                                                     primitiveTopology);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::Pipeline>();
        throw std::runtime_error("Vulkan::Pipeline::CreatePipeline() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::Pipeline>();
        throw std::runtime_error("DirectX::Pipeline::CreatePipeline() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::Pipeline>();
        throw std::runtime_error("Metal::Pipeline::CreatePipeline() is not implemented yet.");
    default:return nullptr;
    }
}

std::shared_ptr<Texture> CreateTexture(const std::shared_ptr<Renderer>& renderer)
{
    switch(renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<OpenGL::Texture>();
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::Texture>();
        throw std::runtime_error("Vulkan::Pipeline::CreateTexture() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::Texture>();
        throw std::runtime_error("DirectX::Pipeline::CreateTexture() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::Texture>();
        throw std::runtime_error("Metal::Pipeline::CreateTexture() is not implemented yet.");
    default:return nullptr;
    }
}

std::vector<std::shared_ptr<Texture>> CreateTextures(const std::shared_ptr<Renderer>& renderer, size_t count)
{
    switch(renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return OpenGL::Texture::CreateTextures(count);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::Texture>();
        throw std::runtime_error("Vulkan::Pipeline::CreateTexture() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::Texture>();
        throw std::runtime_error("DirectX::Pipeline::CreateTexture() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::Texture>();
        throw std::runtime_error("Metal::Pipeline::CreateTexture() is not implemented yet.");
    default:return {};
    }
}

}