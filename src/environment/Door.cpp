#include "environment/Door.hpp"

std::vector<Door*> Door::pool;

Door::Door(Hitbox hb)
{
    this->hitbox = hb;
    Door::pool.push_back(this);
}

void Door::debugDraw(sf::RenderWindow& window)
{
    this->hitbox.debugDraw(window);
}