#include "../include/Entity.hpp"

void Entity::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
}