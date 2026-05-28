#include "environment/Door.hpp"

std::vector<Door*> Door::pool;

Door::Door(size_t id_, Hitbox hb) : id{id_}
{
    this->hitbox = hb;
    Door::pool.push_back(this);
}

void Door::debugDraw(sf::RenderWindow& window)
{
    this->hitbox.debugDraw(window);
}