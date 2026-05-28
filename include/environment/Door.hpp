#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "core/Hitbox.hpp"

class Door
{
public:
    static std::vector<Door*> pool;
    
    size_t id;

    Door* targetDoor;
    Hitbox hitbox;
    
    Door(size_t id, Hitbox hb);

    void debugDraw(sf::RenderWindow& window);
};