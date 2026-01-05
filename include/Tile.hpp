#pragma once
#include <SFML/Graphics.hpp>


struct Tile {
    sf::RectangleShape shape;

    Tile() = default;

    Tile(const sf::Color& color);
};