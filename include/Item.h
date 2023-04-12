//
// Created by Alban on 31/03/2023.
//
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"

enum ItemId : uint16_t
{
    APPLE
};

class Item
{
public:
    [[nodiscard]] inline std::string GetName() const { return m_name; }
    [[nodiscard]] inline int GetStackSize() const { return m_stackSize; }
    [[nodiscard]] inline int GetCount() const { return m_count; }

    virtual void OnUse(class Player& player) = 0;

protected:
    friend class Inventory;
    friend class Hotbar;

    explicit Item(ItemId id) : m_id(id) {}

    ItemId m_id;
    std::string m_name;
    int m_stackSize = 0;
    int m_count = 0;
    TextureRegistry::ResourceHandle m_texture;
};



