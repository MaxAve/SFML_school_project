#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    sf::RectangleShape sprite;
    sf::Vector2f velocity;

    void draw(sf::RenderWindow& window);
};