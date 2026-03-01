#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "gui/SharedInventory.hpp"

class LootContainer {
    float range;
    sf::RectangleShape box;
    Inventory container;

public:

    LootContainer(sf::Vector2u inventorySize, sf::Vector2f _pos, sf::Vector2f _size, float _range);

    float calcDistanceSq(sf::Vector2f coord) const;

    bool inRange(sf::Vector2f coord) const;

    void update(const Player& player, SharedInventory& sharedInventory);

    void draw();

    Inventory* getInventory();
};