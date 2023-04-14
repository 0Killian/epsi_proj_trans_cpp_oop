//
// Created by Killian on 12/04/2023.
//

#pragma once

#include "Item.h"

class Hoe : public Item
{
public:
    Hoe();

    void OnUse(Player& player) override;
};