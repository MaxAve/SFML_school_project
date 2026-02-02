#pragma once

class Hitbox
{
public:
    sf::Vector2f position;
    sf::Vector2f size;

    bool touching(Hitbox* other);
};