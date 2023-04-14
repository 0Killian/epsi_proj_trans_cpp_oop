//
// Created by Killian on 12/04/2023.
//

#pragma once

#include "Item.h"

class WateringCan : public Item
{
public:
    WateringCan();

    void OnUse(Player& player) override;
};