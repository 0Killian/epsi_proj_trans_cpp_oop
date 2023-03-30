//
// Created by Alban on 30/03/2023.
//
#include <Player.h>
#include <Application.h>

void Player::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("player.png");
    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
}

void Player::animation(int index, float timer){
    if(timer < 0.1f){
        m_sprite.setTextureRect(sf::IntRect({index, 64}, {32, 64}));
    } else if(timer < 0.2f){
        m_sprite.setTextureRect(sf::IntRect({index, 128}, {32, 64}));
    } else if (timer < 0.3f) {
        m_sprite.setTextureRect(sf::IntRect({index, 192}, {32, 64}));
    } else if (timer < 0.4f) {
        m_sprite.setTextureRect(sf::IntRect({index, 256}, {32, 64}));
    } else if (timer < 0.5f) {
        m_sprite.setTextureRect(sf::IntRect({index, 320}, {32, 64}));
    } else if (timer > 0.6f) {
        m_animationTimer = 0;
    }
}

void Player::Update(float deltaTime)
{
    // Center sprite on the screen
    m_sprite.setOrigin({16.0f, 48.0f});
    m_sprite.setPosition({Application::WINDOW_WIDTH / 2.f, Application::WINDOW_HEIGHT / 2.f});
    m_sprite.setScale({Application::ZOOM_FACTOR + m_zoomFactor, Application::ZOOM_FACTOR + m_zoomFactor});

    SPDLOG_INFO("m_animationTimer{} {}",m_animationTimer,deltaTime);
    if(m_isMoving){
        m_animationTimer += deltaTime;
        if(m_orientation == UP ){
            animation(32, m_animationTimer);
        }
        if(m_orientation == DOWN){
            animation(0, m_animationTimer);
        }
        if(m_orientation == RIGHT){
            animation(64, m_animationTimer);
        }
        if(m_orientation == LEFT){
            animation(96, m_animationTimer);
        }
    } else {
        if(m_orientation == UP ) {
            m_sprite.setTextureRect(sf::IntRect({32, 0}, {32, 64}));
            m_animationTimer = 0;
        }
        if(m_orientation == DOWN){
            m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
            m_animationTimer = 0;
        }
        if(m_orientation == RIGHT){
            m_sprite.setTextureRect(sf::IntRect({64, 0}, {32, 64}));
            m_animationTimer = 0;
        }
        if(m_orientation == LEFT){
            m_sprite.setTextureRect(sf::IntRect({96, 0}, {32, 64}));
            m_animationTimer = 0;
        }
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}
