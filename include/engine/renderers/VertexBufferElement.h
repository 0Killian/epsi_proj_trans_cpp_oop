//
// Created by Killian on 17/04/2023.
//

#pragma once

class VertexBufferLayout
{
public:
    enum class Type
    {
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static size_t GetSizeOfType(Type type)
    {
        switch (type)
        {
        case Type::Float:
            return 4;
        case Type::Float2:
            return 4 * 2;
        case Type::Float3:
            return 4 * 3;
        case Type::Float4:
            return 4 * 4;
        case Type::Mat3:
            return 4 * 3 * 3;
        case Type::Mat4:
            return 4 * 4 * 4;
        case Type::Int:
            return 4;
        case Type::Int2:
            return 4 * 2;
        case Type::Int3:
            return 4 * 3;
        case Type::Int4:
            return 4 * 4;
        case Type::Bool:
            return 1;
        }

        return 0;
    }

    struct Element
    {
        Type type;
        bool normalized;
        size_t offset;

        explicit Element(Type type, bool normalized = false)
            : type(type), normalized(normalized), offset(0)
        {
        }
    };

    VertexBufferLayout(std::initializer_list<Element> elements)
        : m_elements(elements)
    {
        size_t offset = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += GetSizeOfType(element.type);
        }
        m_stride = offset;
    }

    [[nodiscard]] inline const std::vector<Element>& GetElements() const { return m_elements; }
    [[nodiscard]] inline size_t GetStride() const { return m_stride; }

private:
    uint32_t m_stride;
    std::vector<Element> m_elements;
};

class VertexBufferElement
{
public:
    [[nodiscard]] virtual VertexBufferLayout GetLayout() = 0;
};

template <typename T>
concept IsVertexBufferElement = std::is_base_of_v<VertexBufferElement, T>;
