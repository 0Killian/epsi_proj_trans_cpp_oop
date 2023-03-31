//
// Created by Alban on 31/03/2023.
//

#include "Item.h"

Item::Item(int m_id, std::string m_name, bool m_stackable, int m_max_num, int m_num, bool m_seed, bool m_tool)
        : m_id(m_id), m_name(m_name), m_stackable(m_stackable), m_max_num(m_max_num), m_num(m_num), m_seed(m_seed), m_tool(m_tool)
{
    this->m_id = m_id;
    this->m_name = m_name;
    this->m_stackable = m_stackable;
    this->m_max_num = m_max_num;
    this->m_num = m_num;
    this->m_seed = m_seed;
    this->m_num = m_num;
    this->m_tool = m_tool;
}

std::string Item::getName() const {
    return m_name;
}


