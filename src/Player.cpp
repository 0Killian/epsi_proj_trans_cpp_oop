//
// Created by Alban on 30/03/2023.
//
#include <Player.h>
#include <Application.h>
#include <GameGrid.h>
#include <iostream>
#include <SFML/Window/Joystick.hpp>

void Player::Init()
{
    m_texture = Application::GetInstance().GetTextureRegistry().GetResource("player.png");
    m_sprite.setTexture(m_texture);
    // Center sprite on the screen
    m_sprite.setOrigin({16.0f, 48.0f});
    m_sprite.setPosition({0, 0});
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
    sf::Vector2f move = sf::Vector2f(0, 0);
    if(m_movement != sf::Vector2f(0, 0))
    {
        m_isMoving = true;
        move = m_movement.normalized() * GameGrid::TILE_SIZE * deltaTime
                * (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? SPRINT_SPEED : WALK_SPEED);

        if(m_movement.x < 0)
        {
            m_orientation = Orientation::LEFT;
        }
        else if(m_movement.x > 0)
        {
            m_orientation = Orientation::RIGHT;
        }
        else if(m_movement.y < 0)
        {
            m_orientation = Orientation::UP;
        }
        else if(m_movement.y > 0)
        {
            m_orientation = Orientation::DOWN;
        }
    }
    else
    {
        m_isMoving = false;
    }

    m_position += move;

    m_sprite.setPosition(m_position);

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

bool Player::HandleEvent(const sf::Event &event)
{
    bool blockEvent = false;
    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            m_movement.x -= 1;
            return true;

        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            m_movement.x += 1;
            return true;

        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            m_movement.y -= 1;
            return true;

        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            m_movement.y += 1;
            return true;

        default: break;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            m_movement.x += 1;
            return true;

        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            m_movement.x -= 1;
            return true;

        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            m_movement.y += 1;
            return true;

        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            m_movement.y -= 1;
            return true;

        default: break;
        }
    }
    else if (event.type == sf::Event::JoystickMoved)
    {
        if (event.joystickMove.axis == sf::Joystick::X)
        {
            if (event.joystickMove.position < -20)
            {
                m_movement.x = -1;
            }
            else if (event.joystickMove.position > 20)
            {
                m_movement.x = 1;
            }
            else
            {
                m_movement.x = 0;
            }
        }
        else if (event.joystickMove.axis == sf::Joystick::Y)
        {
            if (event.joystickMove.position < -20)
            {
                m_movement.y = -1;
            }
            else if (event.joystickMove.position > 20)
            {
                m_movement.y = 1;
            }
            else
            {
                m_movement.y = 0;
            }
        }

        return true;
    }

    return blockEvent;
}