#include "core/Hitbox.hpp"

std::vector<Hitbox *> Hitbox::solidHitboxPool;

Hitbox::Hitbox(){}

Hitbox::Hitbox(sf::Vector2f size)
{
    this->size = size;

    debugSprite = sf::RectangleShape(this->size);
    debugSprite.setPosition(this->position);
    debugSprite.setOutlineColor(sf::Color::Red);
    debugSprite.setOutlineThickness(2);
    debugSprite.setFillColor(sf::Color::Transparent);
}

Hitbox::Hitbox(sf::Vector2f position, sf::Vector2f size, bool isSolid)
{
    this->size = size;
    this->position = position;
    this->isSolid = isSolid;
    if(isSolid)
    {
        Hitbox::solidHitboxPool.push_back(this);
    }

    debugSprite = sf::RectangleShape(this->size);
    debugSprite.setPosition(this->position);
    debugSprite.setOutlineColor(sf::Color::Red);
    debugSprite.setOutlineThickness(2);
    debugSprite.setFillColor(sf::Color::Transparent);
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

void Hitbox::debugDraw(sf::RenderWindow &window)
{
    window.draw(this->debugSprite);
}