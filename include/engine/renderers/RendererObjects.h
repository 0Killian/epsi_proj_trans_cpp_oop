//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/opengl/VertexBuffer.h"
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
        return nullptr;
    }
}
