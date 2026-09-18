#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void configure(
        float size,
        float movementSpeed
    );

    void setPosition(
        sf::Vector2f position
    );

    void handleInput(
        float deltaTime
    );

    void move(
        sf::Vector2f offset
    );

    sf::Vector2f
        getPosition() const;

    sf::FloatRect
        getBounds() const;

    void draw(
        sf::RenderWindow& window
    ) const;

private:
    sf::RectangleShape shape;

    float speed;
};