//
// Created by Killian on 18/04/2023.
//
#pragma once

template <typename Controller, typename T>
class Mapping
{
public:
    Mapping(Controller& controller, T* data) : m_controller(controller), m_data(data)
    {
        m_controller.UpdateMappingPointer(this);
    }

    Mapping(const Mapping& other) = delete;
    Mapping(Mapping&& other) noexcept
            : m_controller(other.m_controller), m_data(other.m_data)
    {
        other.m_data = nullptr;
    }

    Mapping& operator=(const Mapping& other) = delete;
    Mapping& operator=(Mapping&& other) noexcept
    {
        m_controller = other.m_buffer;
        m_data = other.m_data;
        other.m_data = nullptr;

        m_controller.UpdateMappingPointer(this);
    }

    ~Mapping()
    {
        if(m_data != nullptr)
            m_controller.Unmap();
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

    T* operator&()
    {
        return m_data;
    }

private:
    Controller& m_controller;
    T* m_data;
};