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
    // Center sprite on the screen
    m_sprite.setOrigin({16.0f, 16.0f});
    m_sprite.setPosition({Application::WINDOW_WIDTH / 2.f, Application::WINDOW_HEIGHT / 2.f});

    // First sprite
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
    m_sprite.setScale({Application::ZOOM_FACTOR + m_zoomFactor, Application::ZOOM_FACTOR + m_zoomFactor});

    if(m_orientation == UP){
        m_sprite.setTextureRect(sf::IntRect({32, 0}, {32, 64}));
    }
    if(m_orientation == DOWN){
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
    }
    if(m_orientation == RIGHT){
        m_sprite.setTextureRect(sf::IntRect({64, 0}, {32, 64}));
    }
    if(m_orientation == LEFT){
        m_sprite.setTextureRect(sf::IntRect({96, 0}, {32, 64}));
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}