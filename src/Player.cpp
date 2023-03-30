//
// Created by Alban on 30/03/2023.
//
#include <Player.h>
#include <Application.h>

void Player::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("player.png");
    m_sprite.setTexture(m_texture);
}

void Player::Update(float deltaTime)
{
    // Centrer le sprite sur l'écran
    m_sprite.setOrigin({16.0f, 16.0f});
    m_sprite.setPosition({Application::WINDOW_WIDTH / 2.f, Application::WINDOW_HEIGHT / 2.f});

    m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));

    m_sprite.setScale({Application::ZOOM_FACTOR + m_zoomFactor, Application::ZOOM_FACTOR + m_zoomFactor});

}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}