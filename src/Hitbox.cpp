#include "../include/Hitbox.hpp"

Hitbox::Hitbox(){}

Hitbox::Hitbox(sf::Vector2f size)
{
    this->size = size;
}

bool Hitbox::touching(Hitbox *other)
{
    return this->withinBounds(other->position)
        || this->withinBounds({other->position.x + other->size.x, other->position.y})
        || this->withinBounds({other->position.x, other->position.y + other->position.y})
        || this->withinBounds({other->position.x + other->size.x, other->position.y + other->size.y});
}

bool Hitbox::withinBounds(sf::Vector2f point)
{
    return point.x >= this->position.x && point.x <= (this->position.x + this->size.x) && point.y >= this->position.y && point.y <= (this->position.y + this->size.y);
}