#pragma once

#include <SFML/Graphics.hpp>

class Hitbox
{
public:
    sf::Vector2f position;
    sf::Vector2f size;

    sf::RectangleShape debugSprite; // Remove later

    Hitbox();
    Hitbox(sf::Vector2f size);
    Hitbox(sf::Vector2f position, sf::Vector2f size);
    bool touching(Hitbox* other);
    bool withinBounds(sf::Vector2f point);
    void debugDraw(sf::RenderWindow& window);
};