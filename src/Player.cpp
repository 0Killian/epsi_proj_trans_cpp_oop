//
// Created by Alban on 30/03/2023.
//
#include <Player.h>
#include <Application.h>
#include <GameGrid.h>
#include <SFML/Window/Joystick.hpp>

void Player::Init()
{
#ifdef DEBUG
    assert(m_gameGrid != nullptr);
    assert(m_hotbar != nullptr);
#endif

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
    sf::Vector2f move = (m_movement + m_movementOffset) * deltaTime;
    if(move != sf::Vector2f(0, 0))
    {
        m_isMoving = true;

        if(move.x < 0)
        {
            m_orientation = Orientation::LEFT;
        }
        else if(move.x > 0)
        {
            m_orientation = Orientation::RIGHT;
        }
        else if(move.y < 0)
        {
            m_orientation = Orientation::UP;
        }
        else if(move.y > 0)
        {
            m_orientation = Orientation::DOWN;
        }
    }
    else
    {
        m_isMoving = false;
    }

    m_position += move;
    m_movementOffset = sf::Vector2f(0, 0);

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

    m_itemCooldown += deltaTime;

    if(m_itemCooldown >= ITEM_COOLDOWN && m_shouldUseItem)
    {
        m_hotbar->UseSelected(*this);
        m_itemCooldown = 0;
    }
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::Rect<float> Player::GetBoundingBox() const
{
    return {
        { m_position.x - 11.0f, m_position.y + 4.0f },
        { 22.0f, 12.0f }
    };
}

bool Player::HandleEvent(const sf::Event &event)
{
    bool moved = false;
    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            m_movementNorm.x = -1;
            moved = true;
            break;

        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            m_movementNorm.x = 1;
            moved = true;
            break;

        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            m_movementNorm.y = -1;
            moved = true;
            break;

        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            m_movementNorm.y = 1;
            moved = true;
            break;

        default: break;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                m_movementNorm.x = 1;
            else
                m_movementNorm.x = 0;
            moved = true;
            break;

        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                m_movementNorm.x = -1;
            else
                m_movementNorm.x = 0;
            moved = true;
            break;

        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                m_movementNorm.y = 1;
            else
                m_movementNorm.y = 0;
            moved = true;
            break;

        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                m_movementNorm.y = -1;
            else
                m_movementNorm.y = 0;
            moved = true;
            break;

        default: break;
        }
    }
    else if (event.type == sf::Event::JoystickMoved)
    {
        if (event.joystickMove.axis == sf::Joystick::X)
        {
            if (event.joystickMove.position < -20)
            {
                m_movementNorm.x = -1;
            }
            else if (event.joystickMove.position > 20)
            {
                m_movementNorm.x = 1;
            }
            else
            {
                m_movementNorm.x = 0;
            }
        }
        else if (event.joystickMove.axis == sf::Joystick::Y)
        {
            if (event.joystickMove.position < -20)
            {
                m_movementNorm.y = -1;
            }
            else if (event.joystickMove.position > 20)
            {
                m_movementNorm.y = 1;
            }
            else
            {
                m_movementNorm.y = 0;
            }
        }

        moved = true;
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        m_shouldUseItem = true;
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        m_shouldUseItem = false;
    }

    if(moved)
    {
        if(m_movementNorm == sf::Vector2f(0, 0))
            m_movement = sf::Vector2f(0, 0);
        else
            m_movement = m_movementNorm.normalized() * GameGrid::TILE_SIZE;

        if(event.type == sf::Event::JoystickMoved)
            m_movement *= sf::Joystick::isButtonPressed(event.joystickMove.joystickId, 1) ? SPRINT_SPEED : WALK_SPEED;
        else
            m_movement *= sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? SPRINT_SPEED : WALK_SPEED;
    }

    return moved;
}