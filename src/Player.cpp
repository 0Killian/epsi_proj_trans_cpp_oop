//
// Created by Alban on 30/03/2023.
//
#include <Player.h>
#include <Application.h>

void Player::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("player.png");
    m_sprite.setTexture(m_texture);
    // Center sprite on the screen
    m_sprite.setOrigin({16.0f, 48.0f});
    m_sprite.setPosition({Application::WINDOW_WIDTH / 2.f, Application::WINDOW_HEIGHT / 2.f});
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 64}));
}

void Player::DoAnimation(int index, float timer)
{
    timer = timer / ANIMATION_DELAY + 1.0f;

    if(timer > ANIMATION_FRAME_COUNT)
    {
        m_animationTimer = 0;
        timer = 1;
    }

    m_sprite.setTextureRect(sf::IntRect(
        {index, static_cast<int>(timer) * HEIGHT},
        {WIDTH, HEIGHT}
    ));
}

void Player::Update(float deltaTime)
{
    // First sprite
    m_sprite.setScale({Application::ZOOM_FACTOR + m_zoomFactor, Application::ZOOM_FACTOR + m_zoomFactor});

    if(m_isMoving)
    {
        m_animationTimer += deltaTime;
        DoAnimation(m_orientation * WIDTH, m_animationTimer);
    }
    else
    {
        m_sprite.setTextureRect(sf::IntRect(
            {m_orientation * WIDTH, 0},
            {WIDTH, HEIGHT}
        ));
        m_animationTimer = 0;
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::Rect<float> Player::GetBoundingBox() const
{
    return {
        { m_position.x - 16, m_position.y - 48 },
        { WIDTH, HEIGHT }
    };
}