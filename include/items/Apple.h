//
// Created by Killian on 12/04/2023.
//

#pragma once

#include "Item.h"

class Apple : public Item
{
public:
    Apple();

    void OnUse(Player& player) override;
};