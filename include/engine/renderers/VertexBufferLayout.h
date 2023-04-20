//
// Created by Killian on 17/04/2023.
//

#pragma once

namespace Engine
{

enum class ShaderBaseType : uint8_t
{
    ShaderBaseType_Float,
    ShaderBaseType_Float2,
    ShaderBaseType_Float3,
    ShaderBaseType_Float4,
    ShaderBaseType_Mat3,
    ShaderBaseType_Mat4,
    ShaderBaseType_Int,
    ShaderBaseType_Int2,
    ShaderBaseType_Int3,
    ShaderBaseType_Int4,
    ShaderBaseType_Bool,
    ShaderBaseType_Texture,
    ShaderBaseType_Unknown
};

class VertexBufferLayout
{
public:
    static size_t GetSizeOfType(ShaderBaseType type)
    {
        switch (type)
        {
        case ShaderBaseType::ShaderBaseType_Float:return 4;
        case ShaderBaseType::ShaderBaseType_Float2:return 4 * 2;
        case ShaderBaseType::ShaderBaseType_Float3:return 4 * 3;
        case ShaderBaseType::ShaderBaseType_Float4:return 4 * 4;
        case ShaderBaseType::ShaderBaseType_Mat3:return 4 * 3 * 3;
        case ShaderBaseType::ShaderBaseType_Mat4:return 4 * 4 * 4;
        case ShaderBaseType::ShaderBaseType_Int:return 4;
        case ShaderBaseType::ShaderBaseType_Int2:return 4 * 2;
        case ShaderBaseType::ShaderBaseType_Int3:return 4 * 3;
        case ShaderBaseType::ShaderBaseType_Int4:return 4 * 4;
        case ShaderBaseType::ShaderBaseType_Bool:return 1;
        }

        return 0;
    }

    static constexpr size_t GetTypeComponentCount(ShaderBaseType type)
    {
        switch (type)
        {
        case ShaderBaseType::ShaderBaseType_Float:return 1;
        case ShaderBaseType::ShaderBaseType_Float2:return 2;
        case ShaderBaseType::ShaderBaseType_Float3:return 3;
        case ShaderBaseType::ShaderBaseType_Float4:return 4;
        case ShaderBaseType::ShaderBaseType_Mat3:return 3 * 3;
        case ShaderBaseType::ShaderBaseType_Mat4:return 4 * 4;
        case ShaderBaseType::ShaderBaseType_Int:return 1;
        case ShaderBaseType::ShaderBaseType_Int2:return 2;
        case ShaderBaseType::ShaderBaseType_Int3:return 3;
        case ShaderBaseType::ShaderBaseType_Int4:return 4;
        case ShaderBaseType::ShaderBaseType_Bool:return 1;
        }

        return 0;
    }

    struct Element
    {
        std::string name;
        ShaderBaseType type;
        bool normalized;
        size_t offset = 0;
        size_t componentCount;

        constexpr Element(std::string name, ShaderBaseType type, bool normalized = false)
            :
            type(type),
            normalized(normalized),
            offset(0),
            name(std::move(name)),
            componentCount(GetTypeComponentCount(type))
        {}

        bool operator==(const Element& other) const
        {
            return name == other.name && type == other.type && normalized == other.normalized && offset == other.offset && componentCount == other.componentCount;
        }
    };

    constexpr VertexBufferLayout(std::initializer_list<Element> elements)
            : m_elements(elements)
    {
        size_t offset = 0;
        for (auto &element: m_elements)
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

template<typename T>
concept IsVertexBufferElement = requires
{
    { T::GetLayout() } -> std::same_as<VertexBufferLayout>;
};

}