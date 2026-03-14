#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class LootContainer {
    float range;
    sf::RectangleShape box;
    bool playerInRange = false;

public:

    LootContainer(sf::Vector2f _pos, sf::Vector2f _size, float _range);

    // ! returns distance as square
    double getDistanceToSq(const sf::Vector2f);

    // ! arg is distance as square
    bool inRangeSq(double);

    void update(const Player& player);

    void draw();
};