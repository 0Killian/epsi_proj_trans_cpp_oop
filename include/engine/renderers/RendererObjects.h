//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/opengl/VertexBuffer.h"
#include "engine/renderers/opengl/IndexBuffer.h"
#include "engine/renderers/opengl/Shader.h"

//#include "renderers/vulkan/VertexBuffer.h"
//#include "renderers/directx/VertexBuffer.h"
//#include "renderers/metal/VertexBuffer.h"

template <typename T>
static std::shared_ptr<::VertexBuffer<T>> CreateVertexBuffer(const std::shared_ptr<Renderer>& renderer, typename ::VertexBuffer<T>::Usage usage)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<OpenGL::VertexBuffer<T>>(usage);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::VertexBuffer<T>>(usage);
        throw std::runtime_error("Vulkan::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::VertexBuffer<T>>(usage);
        throw std::runtime_error("DirectX::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::VertexBuffer<T>>(usage);
        throw std::runtime_error("Metal::VertexBuffer<T>::CreateVertexBuffer() is not implemented yet.");
    default:
        return nullptr;
    }
}

template <typename T>
static std::vector<std::shared_ptr<::VertexBuffer<T>>> CreateVertexBuffers(const std::shared_ptr<Renderer>& renderer, typename ::VertexBuffer<T>::Usage usage, size_t count)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return OpenGL::VertexBuffer<T>::CreateVertexBuffers(usage, count);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::VertexBuffer<T>>(usage);
        throw std::runtime_error("Vulkan::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::VertexBuffer<T>>(usage);
        throw std::runtime_error("DirectX::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::VertexBuffer<T>>(usage);
        throw std::runtime_error("Metal::VertexBuffer<T>::CreateVertexBuffers() is not implemented yet.");
    default:
        return {};
    }
}

static std::shared_ptr<::IndexBuffer> CreateIndexBuffer(const std::shared_ptr<Renderer>& renderer, ::IndexBuffer::Usage usage)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<OpenGL::IndexBuffer>(usage);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::IndexBuffer>(usage);
        throw std::runtime_error("Vulkan::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::IndexBuffer>(usage);
        throw std::runtime_error("DirectX::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::IndexBuffer>(usage);
        throw std::runtime_error("Metal::IndexBuffer::CreateIndexBuffer() is not implemented yet.");
    default:
        return nullptr;
    }
}

static std::vector<std::shared_ptr<::IndexBuffer>> CreateIndexBuffers(const std::shared_ptr<Renderer>& renderer, ::IndexBuffer::Usage usage, size_t count)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return OpenGL::IndexBuffer::CreateIndexBuffers(usage, count);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::IndexBuffer>(usage);
        throw std::runtime_error("Vulkan::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::IndexBuffer>(usage);
        throw std::runtime_error("DirectX::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::IndexBuffer>(usage);
        throw std::runtime_error("Metal::IndexBuffer::CreateIndexBuffers() is not implemented yet.");
    default:
        return {};
    }
}

static std::shared_ptr<::Shader> CreateShader(const std::shared_ptr<Renderer>& renderer, ::Shader::Type type)
{
    switch (renderer->GetAPI())
    {
    case RendererAPI::RendererAPI_OpenGL:
        return std::make_shared<OpenGL::Shader>(type);
    case RendererAPI::RendererAPI_Vulkan:
        //return std::make_shared<Vulkan::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("Vulkan::Shader::CreateShader() is not implemented yet.");
    case RendererAPI::RendererAPI_DirectX:
        //return std::make_shared<DirectX::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("DirectX::Shader::CreateShader() is not implemented yet.");
    case RendererAPI::RendererAPI_Metal:
        //return std::make_shared<Metal::Shader>(vertexShaderSource, fragmentShaderSource);
        throw std::runtime_error("Metal::Shader::CreateShader() is not implemented yet.");
    default:
        return nullptr;
    }
}