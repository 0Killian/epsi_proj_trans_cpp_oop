//
// Created by Killian on 12/04/2023.
//
#include <Application.h>
#include "items/Apple.h"

Apple::Apple() : Item(ItemId::APPLE)
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("items/apple.png");
}

void Apple::OnUse(Player &player)
{
    SPDLOG_INFO("Using Apple !");
}