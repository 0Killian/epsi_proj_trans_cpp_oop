//
// Created by Alban on 31/03/2023.
//

#ifndef STARDEW_INVENTORY_H
#define STARDEW_INVENTORY_H
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"
#include <string>
#include <iostream>
#include <fstream>

class Inventory : public GameObject{
public:
    void Init() override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;

    Inventory();
    void addItem(const Item& item);
    void removeItem(int index);
    void displayItems() const;
    void saveToFile(std::string filename) const;
    void loadFromFile(std::string filename);
private:
    std::vector<Item> items;
    sf::Texture m_texture;
    sf::Sprite m_inventory;
    int count;
};

#endif //STARDEW_INVENTORY_H
