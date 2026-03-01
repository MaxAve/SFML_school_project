#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class LootContainer {
    float range;
    sf::RectangleShape box;

public:

    LootContainer(sf::Vector2f _pos, sf::Vector2f _size, float _range);

    void update(const Player& player);

    void draw();
};