//
// Created by Killian on 17/04/2023.
//
#pragma once

class IndexBuffer
{
public:
    class Mapping
    {
    public:
        Mapping(IndexBuffer& buffer, uint32_t* data) : m_buffer(buffer), m_data(data)
        {
            m_buffer.UpdateMappingPointer(this);
        }

        Mapping(const Mapping& other) = delete;
        Mapping(Mapping&& other) noexcept
                : m_buffer(other.m_buffer), m_data(other.m_data)
        {
            other.m_data = nullptr;
        }

        Mapping& operator=(const Mapping& other) = delete;
        Mapping& operator=(Mapping&& other) noexcept
        {
            m_buffer = other.m_buffer;
            m_data = other.m_data;
            other.m_data = nullptr;

            m_buffer.UpdateMappingPointer(this);
        }

        ~Mapping()
        {
            if(m_data != nullptr)
                m_buffer.Unmap();
        }

        uint32_t& operator[](size_t index)
        {
            return m_data[index];
        }

        uint32_t* operator->()
        {
            return m_data;
        }

        uint32_t& operator*()
        {
            return *m_data;
        }

        uint32_t* operator&()
        {
            return m_data;
        }

    private:
        IndexBuffer& m_buffer;
        uint32_t* m_data;
    };

    enum class Usage : uint8_t
    {
        Static,
        Dynamic,
        Stream
    };

    virtual ~IndexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetData(const uint32_t* data, size_t count) = 0;
    virtual void UpdateData(const uint32_t* data, size_t count, size_t offset) = 0;
    [[nodiscard]] virtual Mapping Map(size_t offset, size_t count) = 0;

    [[nodiscard]] virtual size_t GetCount() = 0;

protected:
    friend Mapping;

    virtual void UpdateMappingPointer(Mapping* mapping) = 0;
    virtual void Unmap() = 0;
};