//
// Created by Killian on 12/04/2023.
//

#pragma once

#include "Item.h"

class Carrot : public Item
{
public:
    Carrot();

    void OnUse(Player& player) override;
};