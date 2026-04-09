#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Hitbox
{
public:
    static std::vector<Hitbox*> solidHitboxPool;

    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape debugSprite; // Maybe make a debug macro to omit this?
    bool isSolid;

    Hitbox();
    Hitbox(sf::Vector2f size);
    Hitbox(sf::Vector2f position, sf::Vector2f size, bool isSolid=false);
    bool touching(Hitbox* other);
    bool withinBounds(sf::Vector2f point);
    void debugDraw(sf::RenderWindow& window);
};