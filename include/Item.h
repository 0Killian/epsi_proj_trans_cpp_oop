//
// Created by Alban on 31/03/2023.
//

#ifndef STARDEW_ITEM_H
#define STARDEW_ITEM_H
#pragma once

#include <GameObject.h>
#include "ResourceRegistry.h"
#include <string>
#include <iostream>
#include <fstream>

class Item {
private:
    int m_id;           // use this for the id of objects
    std::string m_name; // this is the name of the item
    bool m_stackable;   // true indicates that the item can be stacked
    int m_max_num;      // maximum number of items in a stack
    int m_num;          // the current number of items in the stack
    bool m_seed;        // true indicate it is a seed
    bool m_tool;        // true indicate it is a tool so you can use animation
public:
    Item(int m_id, std::string m_name, bool m_stackable, int m_max_num, int m_num, bool m_seed, bool m_tool);
    std::string getName() const;
};


#endif //STARDEW_ITEM_H



