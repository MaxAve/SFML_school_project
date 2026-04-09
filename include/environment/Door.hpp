#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "core/Hitbox.hpp"

class Door
{
public:
    static std::vector<Door*> pool;
    Door* targetDoor;
    Hitbox hitbox;
    
    Door(Hitbox hb);

    void debugDraw(sf::RenderWindow& window);
};