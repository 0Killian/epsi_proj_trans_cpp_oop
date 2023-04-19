//
// Created by Killian on 17/04/2023.
//

#pragma once

#include "engine/renderers/IndexBuffer.h"
#include "engine/glad/glad.h"

namespace Engine::OpenGL
{

class IndexBuffer : public Engine::IndexBuffer
{
public:
    explicit IndexBuffer(IndexBufferUsage usage);
    IndexBuffer(IndexBufferUsage usage, const uint32_t* indices, size_t count);

    IndexBuffer& operator=(const IndexBuffer& other);
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    IndexBuffer(const IndexBuffer& other) { operator=(other); }
    IndexBuffer(IndexBuffer&& other) noexcept { *this = std::move(other); }

    static std::vector<std::shared_ptr<Engine::IndexBuffer>> CreateIndexBuffers(IndexBufferUsage usage, size_t count);

    void Bind() override;
    void Unbind() override;
    void SetData(const uint32_t* indices, size_t count) override;
    void UpdateData(const uint32_t* data, size_t count, size_t offset) override;
    [[nodiscard]] Mapping<Engine::IndexBuffer, uint32_t> Map(size_t offset, size_t count) override;
    [[nodiscard]] inline size_t GetCount() override { return m_count; }

    ~IndexBuffer() override;

protected:
    void Unmap() override;
    inline void UpdateMappingPointer(Mapping<Engine::IndexBuffer, uint32_t>* mapping) override
    {
        m_mapping = mapping;
    }

private:
    IndexBuffer(IndexBufferUsage usage, uint32_t id);

    uint32_t m_id = 0;
    size_t m_count = 0;
    IndexBufferUsage m_usage = IndexBufferUsage::IndexBufferUsage_Static;
    Mapping<Engine::IndexBuffer, uint32_t>* m_mapping = nullptr;
};

}