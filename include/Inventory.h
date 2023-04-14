//
// Created by Alban on 31/03/2023.
//

#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"
#include "Item.h"

class Inventory : public GameObject
{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    bool HandleEvent(const sf::Event& event) override;

    bool SetItem(Item* item, int index = -1);
    [[nodiscard]] inline Item* GetItem(int index) const { return m_items[index]; }
    [[nodiscard]] inline Item* GetHoveredItem() const { return (m_slotHovered != -1) ? m_items[m_slotHovered] : nullptr; }
    [[nodiscard]] inline int GetHoveredSlot() const { return m_slotHovered; }

    static constexpr int HOTBAR_SLOT_COUNT = 10;
    static constexpr int INVENTORY_ROWS = 3;
    static constexpr int INVENTORY_COLUMNS = 10;
    static constexpr int INVENTORY_SLOT_COUNT = INVENTORY_ROWS * INVENTORY_COLUMNS;
    static constexpr int TOTAL_SLOT_COUNT = HOTBAR_SLOT_COUNT + INVENTORY_SLOT_COUNT;

    static constexpr float INVENTORY_WINDOW_WIDTH_PERCENTAGE = 0.8f; // 0-1
    static constexpr float INVENTORY_WINDOW_HEIGHT_PERCENTAGE = 0.8f; // 0-1
    static constexpr int INVENTORY_OFFSET = 10;
    static constexpr int INVENTORY_SLOT_MARGIN = 5;
    static constexpr int INVENTORY_SLOT_SIZE = 32;

private:
    void FindNextFreeSlot();
    void Resize();

    TextureRegistry::ResourceHandle m_texture;
    sf::Sprite m_inventory;

    TextureRegistry::ResourceHandle m_slotHoveredTexture;
    sf::Sprite m_slotHoveredSprite;

    std::mutex m_mutex;
    Item* m_items[TOTAL_SLOT_COUNT] = {nullptr};

    int firstFreeSlot = 0;
    bool m_isOpen = false;
    int m_slotHovered = -1;
    int m_slotInHand = -1;

    float m_slotSize;
    sf::Vector2f m_inventorySize;
    float m_computedOffset;
    float m_computedMargin;
};
