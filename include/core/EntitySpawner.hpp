#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entities/Zombie.hpp"
#include "environment/GameMap.hpp"
#include "entities/Player.hpp"
#include <unordered_set>

class EntitySpawner
{
public:
    static void attemptSpawnZombiesWithinPlayerRadius(Player* player, GameMap* map);
};