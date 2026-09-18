#include "Player.h"

Player::Player()
    : speed(220.0f)
{
    shape.setSize(
        {
            40.0f,
            40.0f
        }
    );

    shape.setFillColor(
        sf::Color::Green
    );
}

void Player::configure(
    float size,
    float movementSpeed
)
{
    shape.setSize(
        {
            size,
            size
        }
    );

    speed =
        movementSpeed;
}

void Player::setPosition(
    sf::Vector2f position
)
{
    shape.setPosition(
        position
    );
}

void Player::handleInput(
    float deltaTime
)
{
    sf::Vector2f movement(
        0.0f,
        0.0f
    );

    if (
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::W
        ) ||
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::Up
        )
        )
    {
        movement.y -=
            speed * deltaTime;
    }

    if (
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::S
        ) ||
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::Down
        )
        )
    {
        movement.y +=
            speed * deltaTime;
    }

    if (
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::A
        ) ||
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::Left
        )
        )
    {
        movement.x -=
            speed * deltaTime;
    }

    if (
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::D
        ) ||
        sf::Keyboard::isKeyPressed(
            sf::Keyboard::Key::Right
        )
        )
    {
        movement.x +=
            speed * deltaTime;
    }

    move(
        movement
    );
}

void Player::move(
    sf::Vector2f offset
)
{
    shape.move(
        offset
    );
}

sf::Vector2f
Player::getPosition() const
{
    return shape.getPosition();
}

sf::FloatRect
Player::getBounds() const
{
    return shape.getGlobalBounds();
}

void Player::draw(
    sf::RenderWindow& window
) const
{
    window.draw(
        shape
    );
}