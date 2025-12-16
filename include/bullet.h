#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "physics.h"

class Bullet
{
public:
    const float DEFAULT_SIZE = 20;

    sf::RectangleShape sprite;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f position, float speed, float direction);

    void draw(sf::RenderWindow& window);

    void update();
};

class 

#endif