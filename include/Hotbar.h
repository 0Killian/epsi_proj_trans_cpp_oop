//
// Created by Killian on 10/04/2023.
//

#pragma once

#include <GameObject.h>
#include "Inventory.h"

class Hotbar : public GameObject
{
public:
    Hotbar(const std::shared_ptr<Inventory>& inventory) : m_inventory(inventory) {};
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    bool HandleEvent(const sf::Event& event) override;

    void UseSelected(class Player& player);

private:
    static constexpr float HOTBAR_WINDOW_WIDTH_PERCENTAGE = 0.8f; // 0-1
    static constexpr float HOTBAR_WINDOW_HEIGHT_PERCENTAGE = 0.1f; // 0-1
    static constexpr int HOTBAR_OFFSET = 10;
    static constexpr int HOTBAR_SLOT_MARGIN = 5;
    static constexpr int HOTBAR_SLOT_SIZE = 32;

    void Resize();
    void SelectSlot(int slot);

    std::shared_ptr<Inventory> m_inventory;
    TextureRegistry::ResourceHandle m_hotbarTexture;
    TextureRegistry::ResourceHandle m_hotbarSlotSelectedTexture;
    sf::Sprite m_hotbarSprite;
    sf::Sprite m_hotbarSlotSelectedSprite;

    int m_selectedSlot = 0;

    float m_slotSize;
    sf::Vector2f m_hotbarSize;
    float m_computedOffset;
    float m_computedMargin;
};
