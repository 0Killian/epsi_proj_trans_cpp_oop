//
// Created by Alban on 31/03/2023.
//

#include "Item.h"
#include "Inventory.h"
#include <Player.h>
#include <Application.h>

void Inventory::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("gui/inventory.png");
    m_inventory.setTexture(m_texture);

    m_slotHoveredTexture = Application::GetInstance().GetTextureRegistry().GetResource("gui/slot.png");
    m_slotHoveredSprite.setTexture(m_slotHoveredTexture);

    Resize();
}

bool Inventory::SetItem(Item* item, int index)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if(index == -1)
    {
        if(firstFreeSlot > 0)
        {
            m_items[firstFreeSlot] = item;
            lock.unlock();
            FindNextFreeSlot();
            return true;
        }
        return false;
    }
    else
    {
        if(index >= 0 && index < TOTAL_SLOT_COUNT)
        {
            m_items[index] = item;
            if(item == nullptr && index < firstFreeSlot)
                firstFreeSlot = index;

            return true;
        }
        return false;
    }
}

void Inventory::FindNextFreeSlot()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    for(int i = 0; i < TOTAL_SLOT_COUNT; i++)
    {
        if(m_items[i] == nullptr)
        {
            firstFreeSlot = i;
            return;
        }
    }
    firstFreeSlot = -1;
}

bool Inventory::HandleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(event.key.code == sf::Keyboard::Tab)
        {
            m_isOpen = !m_isOpen;
            if(!m_isOpen) m_slotHovered = -1;
            return true;
        }
    }
    else if(event.type == sf::Event::Resized)
    {
        Resize();
    }
    else if(event.type == sf::Event::MouseMoved)
    {
        if(m_isOpen)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            sf::Vector2f mousePos = {
                static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y)
            };

            sf::Vector2f inventoryPos = m_inventory.getPosition() + sf::Vector2f(m_computedOffset, m_computedOffset);
            sf::Vector2f inventorySize = m_inventorySize - sf::Vector2f(m_computedOffset * 2, m_computedOffset * 2);

            sf::FloatRect inventoryRect = { inventoryPos, inventorySize };

            if(inventoryRect.contains(mousePos))
            {
                int slotIndexX = static_cast<int>((mousePos.x - inventoryPos.x) / (m_slotSize + m_computedMargin));
                int slotIndexY = static_cast<int>((mousePos.y - inventoryPos.y) / (m_slotSize + m_computedMargin));

                if((mousePos.x - inventoryPos.x) - slotIndexX * (m_slotSize + m_computedMargin) > m_slotSize ||
                   (mousePos.y - inventoryPos.y) - slotIndexY * (m_slotSize + m_computedMargin) > m_slotSize)
                {
                    // Mouse is hovering over a margin
                    m_slotHovered = -1;
                    return false;
                }

                m_slotHovered = slotIndexX + slotIndexY * INVENTORY_COLUMNS;
            }
        }
    }
    return false;
}

void Inventory::Resize()
{
    sf::Vector2f computedSize = {
        static_cast<float>(Application::GetInstance().GetWindowWidth()) * INVENTORY_WINDOW_WIDTH_PERCENTAGE,
        static_cast<float>(Application::GetInstance().GetWindowHeight()) * INVENTORY_WINDOW_HEIGHT_PERCENTAGE
    };

    sf::Vector2f computedScale = {
        computedSize.x / m_texture->getSize().x,
        computedSize.y / m_texture->getSize().y
    };

    sf::Vector2f offset = {
        INVENTORY_OFFSET * computedScale.x * 2,
        INVENTORY_OFFSET * computedScale.y * 2
    };

    sf::Vector2f margin = {
        INVENTORY_SLOT_MARGIN * computedScale.x * (INVENTORY_COLUMNS - 1),
        INVENTORY_SLOT_MARGIN * computedScale.y * (INVENTORY_ROWS - 1)
    };

    sf::Vector2f totalSlotSize = {
        computedSize.x - (offset.x + margin.x),
        computedSize.y - (offset.y + margin.y)
    };

    sf::Vector2f slotSize = {
        totalSlotSize.x / INVENTORY_COLUMNS,
        totalSlotSize.y / INVENTORY_ROWS
    };

    m_slotSize = std::min(slotSize.x, slotSize.y);
    float finalScale = m_slotSize / INVENTORY_SLOT_SIZE;
    m_computedMargin = INVENTORY_SLOT_MARGIN * finalScale;
    m_computedOffset = INVENTORY_OFFSET * finalScale;

    m_inventorySize = {
        m_slotSize * INVENTORY_COLUMNS + m_computedOffset * 2 + m_computedMargin * (INVENTORY_COLUMNS - 1),
        m_slotSize * INVENTORY_ROWS + m_computedOffset * 2 + m_computedMargin * (INVENTORY_ROWS - 1)
    };

    m_inventory.setScale({
         m_inventorySize.x / m_texture->getSize().x,
         m_inventorySize.y / m_texture->getSize().y
     });

    m_inventory.setPosition({
        Application::GetInstance().GetWindowWidth() / 2 - m_inventorySize.x / 2,
        Application::GetInstance().GetWindowHeight() / 2 - m_inventorySize.y / 2
    });

    m_slotHoveredSprite.setScale({
        finalScale,
        finalScale
    });
}

void Inventory::Update(float deltaTime)
{

}

void Inventory::Render(sf::RenderWindow& window)
{
    if(!m_isOpen) return;

    window.draw(m_inventory);

    std::unique_lock<std::mutex> lock(m_mutex);
    if(m_slotHovered != -1)
    {
        m_slotHoveredSprite.setPosition({
            m_inventory.getPosition().x + m_computedOffset + (m_slotHovered % INVENTORY_COLUMNS) * (m_slotSize + m_computedMargin),
            m_inventory.getPosition().y + m_computedOffset + (m_slotHovered / INVENTORY_COLUMNS) * (m_slotSize + m_computedMargin)
        });

        window.draw(m_slotHoveredSprite);
    }

    for(int y = 0; y < INVENTORY_ROWS; y++)
    {
        for(int x = 0; x < INVENTORY_COLUMNS; x++)
        {
            int index = y * INVENTORY_COLUMNS + x;
            Item* item = m_items[index];
            if(item != nullptr)
            {
                TextureRegistry::ResourceHandle texture = item->m_texture;
                sf::Sprite sprite = sf::Sprite(texture);
                sprite.setPosition({
                    m_inventory.getPosition().x + m_computedOffset + x * (m_slotSize + m_computedMargin),
                    m_inventory.getPosition().y + m_computedOffset + y * (m_slotSize + m_computedMargin)
                });
                sprite.setScale({
                    m_slotSize / texture->getSize().x,
                    m_slotSize / texture->getSize().y
                });
                window.draw(sprite);
            }
        }
    }
}