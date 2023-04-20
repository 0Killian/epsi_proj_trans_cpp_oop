//
// Created by Killian on 18/04/2023.
//
#pragma once

namespace Engine
{

template<typename Controller, typename T>
requires (!std::move_constructible<Controller>)
class Mapping
{
protected:
    friend Controller;

    template <typename U, typename V>
    requires (!std::move_constructible<U>)
    friend class Mapping;

    Mapping(Controller* controller, T* data) : m_controller(controller), m_data(data) {}

public:
    template <typename U>
    requires std::is_base_of_v<Controller, U>
    Mapping(Mapping<U, T>&& other) // NOLINT
    {
        m_controller = static_cast<U*>(other.m_controller);
        m_data = other.m_data;
        other.m_data = nullptr;
        other.m_controller = nullptr;
    }

    Mapping(const Mapping& other) = delete;

    Mapping(Mapping&& other) noexcept
            : m_controller(other.m_controller), m_data(other.m_data)
    {
        other.m_data = nullptr;
        other.m_controller = nullptr;
    }

    Mapping& operator=(const Mapping& other) = delete;

    Mapping& operator=(Mapping&& other) noexcept
    {
        m_controller = other.m_buffer;
        m_data = other.m_data;
        other.m_data = nullptr;
        other.m_controller = nullptr;
    }

    ~Mapping()
    {
        if (m_controller != nullptr)
            m_controller->Unmap();
    }

    T& operator[](size_t index)
    {
        return m_data[index];
    }

    T* operator->()
    {
        return m_data;
    }

    T& operator*()
    {
        return *m_data;
    }

    T* operator&() // NOLINT
    {
        return m_data;
    }

protected:
    Controller* m_controller;
    T* m_data;
};

}