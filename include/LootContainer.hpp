#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include <vector>
#include "InventorySlot.hpp"

class LootContainer {
    float range;
    sf::RectangleShape box;
    bool playerInRange = false;

    std::vector<std::vector<InventorySlot>> inventorySlots; // [x][y]

public:

    LootContainer(sf::Vector2f _pos, sf::Vector2f _size, float _range);

    // ! returns distance as square
    double getDistanceToSq(const sf::Vector2f);

    std::vector<std::vector<InventorySlot>>* getInventorySlots();

    // ! arg is distance as square
    bool inRangeSq(double);

    // TODO: bool isPlayerInRange();

    void update(const Player& player);

    void draw();
};