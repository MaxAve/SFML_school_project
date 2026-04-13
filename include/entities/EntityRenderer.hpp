#pragma once

#include <iostream>
#include <utility>
#include <algorithm>
#include "Player.hpp"
#include "Zombie.hpp"

enum EntityType
{
    PLAYER,
    ZOMBIE,
};

class EntityRenderer
{
public:
    static std::vector<std::pair<EntityType, void*>> entities;

    static bool compareEntities(std::pair<EntityType, void*> a, std::pair<EntityType, void*> b);
    static void sortEntities(Player* player, std::vector<Zombie*>& zombiePool);
    static void drawAll(sf::RenderWindow& window);
};